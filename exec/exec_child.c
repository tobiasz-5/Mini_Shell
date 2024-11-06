/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor7_redir_child.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: negambar <negambar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:45:51 by girindi          #+#    #+#             */
/*   Updated: 2024/11/05 16:10:38 by negambar         ###   ########.fr       */
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
			fprintf(stderr, "Open heredc %s\n", redirection->heredoc_filename);
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
