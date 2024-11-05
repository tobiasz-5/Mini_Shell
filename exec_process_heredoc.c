/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_process_heredoc.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girindi <girindi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:03:08 by girindi           #+#    #+#             */
/*   Updated: 2024/11/05 16:24:40 by girindi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"
#define MAX_LINE_LENGTH 4096

void	heredoc_input_loop(const char	*delimiter, int fd,
			bool is_quoted, t_shell_state *shell_state)
{
	char	line[MAX_LINE_LENGTH];
	size_t	line_len;
	bool	should_exit;

	should_exit = false;
	while (1)
	{
		write(1, "> ", 2);
		should_exit = read_input_line(line, &line_len);
		if (should_exit || g_received_signal == SIGINT)
		{
			write(1, "\n", 1);
			break ;
		}
		if (strcmp(line, delimiter) == 0)
			break ;
		expand_and_write_line(line, fd, is_quoted, shell_state);
	}
}

int	process_all_heredocs(t_command *command_list, t_shell_state *shell_state)
{
	t_command		*current_cmd;

	current_cmd = command_list;
	while (current_cmd)
	{
		if (process_command_heredocs(current_cmd, shell_state) < 0)
			return (1);
		current_cmd = current_cmd->next;
	}
	return (0);
}

int	process_command_heredocs(t_command *current_cmd, t_shell_state *shell_state)
{
	t_redirection	*redirection;
	char			*heredoc_filename;

	redirection = current_cmd->redirections;
	while (redirection)
	{
		if (redirection->type == TOKEN_HEREDOC)
		{
			heredoc_filename = NULL;
			handle_heredoc(redirection, &heredoc_filename, shell_state);
			if (!heredoc_filename)
			{
				write(2, "failed to create heredoc file 4 redir\n", 38);
				return (-1);
			}
			redirection->heredoc_filename = heredoc_filename;
		}
		redirection = redirection->next;
	}
	return (0);
}

void	handle_heredoc(t_redirection *redirection,
			char **heredoc_filename, t_shell_state *shell_state)
{
	int	fd;

	fd = create_temp_heredoc_file(heredoc_filename);
	if (fd == -1)
		return ;
	read_and_expand_heredoc(redirection->filename, fd,
		redirection->is_quoted, shell_state);
	close(fd);
}

void	read_and_expand_heredoc(const char *delimiter, int fd,
				bool is_quoted, t_shell_state *shell_state)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;
	struct sigaction	sa_old_int;
	struct sigaction	sa_old_quit;

	sa_int.sa_handler = handle_sigint_heredoc;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, &sa_old_int);
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, &sa_old_quit);
	heredoc_input_loop(delimiter, fd, is_quoted, shell_state);
	sigaction(SIGINT, &sa_old_int, NULL);
	sigaction(SIGQUIT, &sa_old_quit, NULL);
}