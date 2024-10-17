/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor6_redir_builtin_utils.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:45:49 by tschetti          #+#    #+#             */
/*   Updated: 2024/10/15 00:58:24 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

int	apply_redirection(t_redirection *redirection, t_io_fds *fds,
					t_shell_state *shell_state)
{
	if (redirection->type == TOKEN_REDIR_IN)
		return (redirect_input(redirection, fds));
	else if (redirection->type == TOKEN_REDIR_OUT
		|| redirection->type == TOKEN_REDIR_APPEND)
		return (redirect_output(redirection, fds));
	else if (redirection->type == TOKEN_HEREDOC)
		return (handle_heredoc_redirection(redirection, fds, shell_state));
	return (0);
}
