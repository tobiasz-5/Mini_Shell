/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 10:08:03 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/27 10:08:05 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

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
		return (-999);
	}
	return (fd);
}

int	process_input_redirection(t_redirection *redirection,
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
		filename = redirection->heredoc_filename;
	}
	else
		return (1);
	*last_input_fd = open_input_file(filename);
	if (*last_input_fd == -999)
		return (0);
	return (1);
}

int	process_redirection(t_redirection *redirection,
			int *last_input_fd, t_io_fds *fds, t_shell_state *shell_state)
{
	if (redirection->type == TOKEN_REDIR_IN
		|| redirection->type == TOKEN_HEREDOC)
	{
		if (!process_input_redirection(redirection, last_input_fd))
			return (0);
	}
	else if (redirection->type == TOKEN_REDIR_OUT
		|| redirection->type == TOKEN_REDIR_APPEND)
	{
		if (apply_redirection(redirection, fds, shell_state) < 0)
		{
			exit(EXIT_FAILURE);
		}
	}
	return (1);
}

int	handle_child_redirections(t_command *command,
			t_io_fds *fds, t_shell_state *shell_state)
{
	t_redirection	*redirection;
	int				last_input_fd;

	redirection = command->redirections;
	last_input_fd = -1;
	while (redirection)
	{
		if (!process_redirection(redirection, &last_input_fd, fds, shell_state))
			return (0);
		redirection = redirection->next;
	}
	finalize_input_redirection(last_input_fd);
	return (1);
}
