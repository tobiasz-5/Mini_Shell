/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_apply.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 18:35:07 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/27 18:53:48 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
gestisce redir heredoc
-apre file temp creato per heredoc (onlyread-> 1 solo lato lett.)
-se open da errore, perror, e ritorna -1
-chiama dup2 per duplicare l'fd per l'heredoc su stdin
-chiude fd per heredoc e cancella file tmp(unlink)
*/
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

/*
applica redir in base al tipo
altrimenti ritorna 0 a i chiamanti handle_builtin_redirections
e process_redirection che in tal caso non fanno nulla
*/
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
