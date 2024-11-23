/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor6_redir_builtin_utils.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: negambar <negambar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:45:49 by girindi          #+#    #+#             */
/*   Updated: 2024/11/05 16:10:38 by negambar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

int	apply_heredoc_file(t_redirection *redirection)
{
	int	fd;

	fd = open(redirection->heredoc_filename, O_RDONLY);
	if (fd < 0)
	{
		perror("open heredoc file");
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2 heredoc file");
		close(fd);
		return (-1);
	}
	close(fd);
	unlink(redirection->heredoc_filename);
	return (0);
}

int	apply_redirection(t_redirection *redirection, t_io_fds *fds,
			t_shell_state *shell_state)
{
	(void)shell_state;
	if (redirection->type == TOKEN_REDIR_IN)
		return (redirect_input(redirection, fds));
	else if (redirection->type == TOKEN_REDIR_OUT
		|| redirection->type == TOKEN_REDIR_APPEND)
		return (redirect_output(redirection, fds));
	else if (redirection->type == TOKEN_HEREDOC)
		return (apply_heredoc_file(redirection));
	return (0);
}
