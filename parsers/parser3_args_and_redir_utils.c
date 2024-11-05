/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser3_args_and_redir_utils.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: negambar <negambar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:46:47 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/05 16:17:16 by negambar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

int	is_redirection_token(t_token_type type)
{
	if (type == TOKEN_REDIR_IN)
		return (1);
	if (type == TOKEN_REDIR_OUT)
		return (1);
	if (type == TOKEN_REDIR_APPEND)
		return (1);
	if (type == TOKEN_HEREDOC)
		return (1);
	return (0);
}
