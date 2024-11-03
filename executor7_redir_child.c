/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor7_redir_child.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:45:51 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/02 15:02:03 by tschetti         ###   ########.fr       */
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

int	create_and_open_heredoc(t_redirection *redirection,
		t_shell_state *shell_state, int *heredoc_fd, char **heredoc_filename)
{
	*heredoc_filename = NULL;
	handle_heredoc(redirection, heredoc_filename, shell_state);
	if (*heredoc_filename == NULL)
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
	*heredoc_fd = open(*heredoc_filename, O_RDONLY);
	if (*heredoc_fd < 0)
	{
		perror("Error opening heredoc file");
		free(*heredoc_filename);
		return (-1);
	}
	return (0);
}

int	perform_heredoc_redirection2(int heredoc_fd, char *heredoc_filename)
{
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

int	handle_heredoc_redirection2(t_redirection *redirection, t_io_fds *fds,
			t_shell_state *shell_state)
{
	char	*heredoc_filename;
	int		heredoc_fd;

	if (backup_fd(STDIN_FILENO, &fds->stdin_backup,
			"Error backing up stdin for heredoc") < 0)
		return (-1);
	if (create_and_open_heredoc(redirection, shell_state,
			&heredoc_fd, &heredoc_filename) < 0)
		return (-1);
	if (perform_heredoc_redirection2(heredoc_fd, heredoc_filename) < 0)
		return (-1);
	return (0);
}

void	finalize_input_redirection(int last_input_fd)
{
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

int	open_input_file(const char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("Error opening input file");
		exit(EXIT_FAILURE);
	}
	return (fd);
}

void	process_input_redirection(t_redirection *redirection,
			int *last_input_fd)
{
	const char	*filename;

	filename = NULL;
	if (*last_input_fd != -1)
		close(*last_input_fd);
	if (redirection->type == TOKEN_REDIR_IN)
		filename = redirection->filename;
	else if (redirection->type == TOKEN_HEREDOC)
	{
		if (redirection->heredoc_filename == NULL)
		{
			write(2, "Errorheoc_filme NULL hdle_child_redions\n", 41);
			exit(EXIT_FAILURE);
		}
		else
			fprintf(stderr, "Opening heredoc file: %s\n", redirection->heredoc_filename);
		filename = redirection->heredoc_filename;
	}
	else
		return ;
	*last_input_fd = open_input_file(filename);
}

void	process_redirection(t_redirection *redirection,
			int *last_input_fd, t_io_fds *fds, t_shell_state *shell_state)
{
	if (redirection->type == TOKEN_REDIR_IN
		|| redirection->type == TOKEN_HEREDOC)
	{
		process_input_redirection(redirection, last_input_fd);
	}
	else if (redirection->type == TOKEN_REDIR_OUT
		|| redirection->type == TOKEN_REDIR_APPEND)
	{
		if (apply_redirection(redirection, fds, shell_state) < 0)
		{
			exit(EXIT_FAILURE);
		}
	}
}

void	handle_child_redirections(t_command *command,
			t_io_fds *fds, t_shell_state *shell_state)
{
	t_redirection	*redirection;
	int				last_input_fd;

	redirection = command->redirections;
	last_input_fd = -1;
	while (redirection)
	{
		process_redirection(redirection, &last_input_fd, fds, shell_state);
		redirection = redirection->next;
	}
	finalize_input_redirection(last_input_fd);
}
