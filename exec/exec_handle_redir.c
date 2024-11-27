/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handle_redir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 10:07:43 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/27 18:23:52 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
queste funzioni non servono piu, tenute per sicurezza
*/
int	open_file(const char *filename, int flags, mode_t mode)
{
	int	fd;

	fd = open(filename, flags, mode);
	if (fd < 0)
		perror("Error opening file");
	return (fd);
}

int	apply_dup2(int fd, int target_fd)
{
	if (dup2(fd, target_fd) < 0)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	process_single_redirection(t_redirection *redirection, t_io_fds *fds,
					t_shell_state *shell_state)
{
	if (redirection->type == TOKEN_REDIR_IN)
		return (handle_input_redirection(redirection));
	else if (redirection->type == TOKEN_REDIR_OUT)
		return (hndl_out_redir(redirection, O_WRONLY | O_CREAT | O_TRUNC));
	else if (redirection->type == TOKEN_REDIR_APPEND)
		return (hndl_out_redir(redirection, O_WRONLY | O_CREAT | O_APPEND));
	else if (redirection->type == TOKEN_HEREDOC)
		return (handle_heredoc_redirection2(redirection, fds, shell_state));
	return (0);
}
