/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errs3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 10:49:20 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/28 10:49:22 by tschetti         ###   ########.fr       */
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
