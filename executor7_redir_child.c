/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor7_redir_child.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:45:51 by tschetti          #+#    #+#             */
/*   Updated: 2024/10/15 13:40:27 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

int	handle_input_redirection(t_redirection *redirection)
{
	int	fd;

	fd = open_file(redirection->filename, O_RDONLY, 0);
	if (fd < 0)
		return (-1);
	return (apply_dup2(fd, STDIN_FILENO));
}

int	hndl_out_redir(t_redirection *redirection, int flags)
{
	int	fd;

	fd = open_file(redirection->filename, flags, 0644);
	if (fd < 0)
		return (-1);
	return (apply_dup2(fd, STDOUT_FILENO));
}

int	handle_heredoc_redirection2(t_redirection *redirection, t_io_fds *fds,
					t_shell_state *shell_state)
{
	if (backup_fd(STDIN_FILENO, &fds->stdin_backup,
			"Error backing up stdin for heredoc") < 0)
		return (-1);
	handle_heredoc(redirection->filename, &fds->heredoc_fd,
		redirection->is_quoted, shell_state);
	if (fds->heredoc_fd < 0)
	{
		if (g_received_signal != 0)
		{
			errno = EINTR;
			perror("Error creating heredoc");
		}
		else
		{
			perror("Error creating heredoc");
		}
		return (-1);
	}
	return (apply_dup2(fds->heredoc_fd, STDIN_FILENO));
}

void	handle_child_redirections(t_command *command, t_io_fds *fds,
					t_shell_state *shell_state)
{
	t_redirection	*redirection;

	redirection = command->redirections;
	while (redirection)
	{
		if (apply_redirection(redirection, fds, shell_state) < 0)
		{
			if (fds->stdin_backup != -1)
				close(fds->stdin_backup);
			if (fds->stdout_backup != -1)
				close(fds->stdout_backup);
			exit(EXIT_FAILURE);
		}
		redirection = redirection->next;
	}
}
