/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec5_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: negambar <negambar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:45:47 by girindi          #+#    #+#             */
/*   Updated: 2024/11/05 16:27:25 by negambar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

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

int	perform_heredoc_redirection(const char *heredoc_filename)
{
	int	heredoc_fd;

	heredoc_fd = open(heredoc_filename, O_RDONLY);
	if (heredoc_fd < 0)
	{
		perror ("error opening heredoc file");
		return (-1);
	}
	if (dup2(heredoc_fd, STDIN_FILENO) < 0)
	{
		perror("error redirecting stdin for heredoc");
		close(heredoc_fd);
		return (-1);
	}
	close (heredoc_fd);
	unlink(heredoc_filename);
	return (0);
}

int	handle_heredoc_redirection(t_redirection *redirection, t_io_fds *fds,
			t_shell_state *shell_state)
{
	char	*heredoc_filename;

	heredoc_filename = NULL;
	if (backup_fd(STDIN_FILENO, &fds->stdin_backup,
			"Error backing up stdin for heredoc") < 0)
		return (-1);
	handle_heredoc(redirection, &heredoc_filename, shell_state);
	if (heredoc_filename == NULL)
	{
		write(2, "Error handling heredoc\n", 23);
		return (-1);
	}
	if (perform_heredoc_redirection(heredoc_filename) < 0)
	{
		free(heredoc_filename);
		return (-1);
	}
	free(heredoc_filename);
	return (0);
}
