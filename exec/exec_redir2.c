/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 10:06:40 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/27 18:48:27 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
Queste funzioni non servono piu, lasciate qui per ora,
in exec_redirect.c le equivalenti giuste, fatte bene
*/
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
