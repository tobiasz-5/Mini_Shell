/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor7_redir_child.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:45:51 by tschetti          #+#    #+#             */
/*   Updated: 2024/10/29 12:06:45 by tschetti         ###   ########.fr       */
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
	char	*heredoc_filename;
	int		heredoc_fd;

	heredoc_filename = NULL;
	if (backup_fd(STDIN_FILENO, &fds->stdin_backup,
			"Error backing up stdin for heredoc") < 0)
		return (-1);
	handle_heredoc(redirection, &heredoc_filename, shell_state);
	if (heredoc_filename == NULL)
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
	heredoc_fd = open(heredoc_filename, O_RDONLY);
	if (heredoc_fd < 0)
	{
		perror("Error opening heredoc file");
		free(heredoc_filename);
		return (-1);
	}
	if (dup2(heredoc_fd, STDIN_FILENO) < 0)
	{
		perror("Error redirecting stdin for heredoc");
		close(heredoc_fd);
		free(heredoc_filename);
		return (-1);
	}
	close(heredoc_fd);
	unlink(heredoc_filename);
	free(heredoc_filename);
	return (0);
}

void	handle_child_redirections(t_command *command, t_io_fds *fds, t_shell_state *shell_state)
{
	t_redirection	*redirection;
	int				last_input_fd;

	redirection = command->redirections;
	last_input_fd = -1;
	while (redirection)
	{
		if (redirection->type == TOKEN_REDIR_IN || redirection->type == TOKEN_HEREDOC)
		{
			if (last_input_fd != -1)
			{
				close(last_input_fd);
			}
			if (redirection->type == TOKEN_REDIR_IN)
			{
				last_input_fd = open(redirection->filename, O_RDONLY);
				if (last_input_fd < 0)
				{
					perror("Error opening input file");
					exit(EXIT_FAILURE);
				}
			}
			else if (redirection->type == TOKEN_HEREDOC)
			{
				if (redirection->heredoc_filename == NULL)
				{
					fprintf(stderr, "Error: heredoc_filename is NULL in handle_child_redirections\n");
					exit(EXIT_FAILURE);
				}
				else
				{
					printf("handle_child_redirections: heredoc_filename = %s\n", redirection->heredoc_filename);
				}
				last_input_fd = open(redirection->heredoc_filename, O_RDONLY);
				if (last_input_fd < 0)
				{
					perror("Error opening heredoc file");
					exit(EXIT_FAILURE);
				}
			}
		}
		else if (redirection->type == TOKEN_REDIR_OUT || redirection->type == TOKEN_REDIR_APPEND)
		{
			if (apply_redirection(redirection, fds, shell_state) < 0)
			{
				exit(EXIT_FAILURE);
			}
		}
		redirection = redirection->next;
	}
	if (last_input_fd != -1)
	{
		if (dup2(last_input_fd, STDIN_FILENO) < 0)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(last_input_fd);
	}
}
