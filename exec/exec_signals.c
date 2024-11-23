/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girindi <girindi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 23:42:51 by girindi          #+#    #+#             */
/*   Updated: 2024/11/05 16:59:29 by girindi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

void	handle_signals_in_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
}

void	restore_signals_after_command(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = handle_sigquit;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	handle_signals_in_parent(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	handle_sigint_heredoc(int sig)
{
	g_received_signal = sig;
}

void	handle_fork(t_command *all_cmds, t_fork_info *finfo,
			t_shell_state *shell_state)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		handle_signals_in_child();
		exec_in_child(all_cmds, finfo, shell_state);
	}
	else if (pid > 0)
	{
		handle_signals_in_parent();
		handle_parent_after_fork(pid, shell_state);
		restore_signals_after_command();
	}
	else
	{
		perror("fork");
		shell_state->last_exit_status = 1;
	}
}
