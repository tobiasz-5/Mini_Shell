/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errs3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: negambar <negambar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:45:38 by girindi          #+#    #+#             */
/*   Updated: 2024/11/05 16:16:25 by negambar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

void	init_var_for_parse_redirection(t_redirection *redirection,
					t_parser_state *state)
{
	redirection->filename = NULL;
	redirection->type = state->current_token->type;
	state->current_token = state->current_token->next;
}

void	init_vars_for_strdup_err(t_parser_state *state,
				t_redirection *redirection)
{
	state->flg_error = true;
	state->error_msg = "Malloc failed from strdup";
	free(redirection);
}

void	init_var_expctd_file_or_heredoc(t_parser_state *state,
				t_redirection *redirection)
{
	state->flg_error = true;
	state->error_msg = "Expected filename or delimiter for heredoc";
	free(redirection);
}
