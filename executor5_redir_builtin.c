/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor5_redir_builtin.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:45:47 by tschetti          #+#    #+#             */
/*   Updated: 2024/10/15 13:53:31 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

int	backup_fd(int old_fd, int *backup_fd, const char *error_msg)
{
	if (*backup_fd == -1)
	{
		*backup_fd = dup(old_fd);
		if (*backup_fd < 0)
		{
			perror(error_msg);
			return (-1);
		}
	}
	return (0);
}

int	handle_builtin_redirections(t_command *command, t_io_fds *fds,
					t_shell_state *shell_state)
{
	t_redirection	*redirection;

	redirection = command->redirections;
	while (redirection)
	{
		if (apply_redirection(redirection, fds, shell_state) < 0)
			return (-1);
		redirection = redirection->next;
	}
	return (0);
}

int	redirect_input(t_redirection *redirection, t_io_fds *fds)
{
	if (backup_fd(STDIN_FILENO, &fds->stdin_backup, "Err backing up stdin") < 0)
		return (-1);
	fds->infile_fd = open(redirection->filename, O_RDONLY);
	if (fds->infile_fd < 0)
	{
		perror("Error opening input file");
		return (-1);
	}
	if (dup2(fds->infile_fd, STDIN_FILENO) < 0)
	{
		perror("Error redirecting stdin");
		close(fds->infile_fd);
		return (-1);
	}
	close(fds->infile_fd);
	return (0);
}

int	redirect_output(t_redirection *redirection, t_io_fds *fds)
{
	int	flags;

	if (redirection->type == TOKEN_REDIR_OUT)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else if (redirection->type == TOKEN_REDIR_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		return (-1);
	if (backup_fd(STDOUT_FILENO, &fds->stdout_backup,
			"Error backing up stdout") < 0)
		return (-1);
	fds->outfile_fd = open(redirection->filename, flags, 0644);
	if (fds->outfile_fd < 0)
	{
		perror("Error opening output file");
		return (-1);
	}
	if (dup2(fds->outfile_fd, STDOUT_FILENO) < 0)
	{
		perror("Error redirecting stdout");
		close(fds->outfile_fd);
		return (-1);
	}
	close(fds->outfile_fd);
	return (0);
}

int	handle_heredoc_redirection(t_redirection *redirection, t_io_fds *fds,
					t_shell_state *shell_state)
{
	if (backup_fd(STDIN_FILENO, &fds->stdin_backup,
			"Error backing up stdin for heredoc") < 0)
		return (-1);
	handle_heredoc(redirection->filename, &fds->heredoc_fd,
		redirection->is_quoted, shell_state);
	if (fds->heredoc_fd < 0)
	{
		write(2, "Exited with Signal\n", 19);
		return (-1);
	}
	if (dup2(fds->heredoc_fd, STDIN_FILENO) < 0)
	{
		perror("Error redirecting stdin for heredoc");
		close(fds->heredoc_fd);
		return (-1);
	}
	close(fds->heredoc_fd);
	return (0);
}
