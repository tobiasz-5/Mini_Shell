/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor9_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:45:55 by tschetti          #+#    #+#             */
/*   Updated: 2024/10/17 03:33:57 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

#define MAX_LINE_LENGTH 4096

void	handle_sigint_heredoc(int sig)
{
	g_received_signal = sig;
}

static void	setup_signal_handlers(struct sigaction *sa_int,
				struct sigaction *sa_quit,
				struct sigaction *old_sa_int,
				struct sigaction *old_sa_quit)
{
	sa_int->sa_handler = handle_sigint_heredoc;
	sigemptyset(&sa_int->sa_mask);
	sa_int->sa_flags = 0;
	sigaction(SIGINT, sa_int, old_sa_int);
	sa_quit->sa_handler = SIG_IGN;
	sigemptyset(&sa_quit->sa_mask);
	sa_quit->sa_flags = 0;
	sigaction(SIGQUIT, sa_quit, old_sa_quit);
}

static void	restore_signal_handlers(struct sigaction *old_sa_int,
				struct sigaction *old_sa_quit)
{
	sigaction(SIGINT, old_sa_int, NULL);
	sigaction(SIGQUIT, old_sa_quit, NULL);
}

static bool	setup_pipe(int pipefd[2], int *heredoc_fd)
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		*heredoc_fd = -1;
		return (false);
	}
	*heredoc_fd = pipefd[0];
	return (true);
}

void	handle_heredoc(const char *delimiter, int *heredoc_fd,
			bool is_quoted, t_shell_state *shell_state)
{
	int					pipefd[2];
	struct sigaction	sa_int;
	struct sigaction	sa_quit;
	struct sigaction	old_sa_int;
	struct sigaction	old_sa_quit;

	if (!setup_pipe(pipefd, heredoc_fd))
		return ;
	g_received_signal = 0;
	setup_signal_handlers(&sa_int, &sa_quit, &old_sa_int, &old_sa_quit);
	read_and_expand_heredoc(delimiter, pipefd, is_quoted, shell_state);
	close(pipefd[1]);
	restore_signal_handlers(&old_sa_int, &old_sa_quit);
	if (g_received_signal == SIGINT)
	{
		close(pipefd[0]);
		*heredoc_fd = -1;
	}
}
