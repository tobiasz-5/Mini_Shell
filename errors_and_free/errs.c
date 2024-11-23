/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: negambar <negambar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:45:34 by girindi          #+#    #+#             */
/*   Updated: 2024/11/05 16:27:25 by negambar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

void	init_var_for_parsing_tokens(t_token_node *tokens, t_parser_state *state,
			t_command **cmd_list, t_command **last_command)
{
	*cmd_list = NULL;
	*last_command = NULL;
	state->current_token = tokens;
	state->flg_error = false;
	state->error_msg = NULL;
}

void	call_error_and_free(t_parser_state *state, t_command **cmd_list)
{
	printf("Error: %s\n", state->error_msg);
	free_command(*cmd_list);
}

void	init_vars_for_parsing_command(t_command *command)
{
	command->cmd_name = NULL;
	command->has_dollar = false;
	command->args_list = NULL;
	command->redirections = NULL;
	command->next = NULL;
}

void	init_vars_for_not_cmd(t_parser_state *state)
{
	state->flg_error = true;
	state->error_msg = "Malloc failed";
}

void	init_vars_for_cmdname_error(t_parser_state *state, t_command *command)
{
	state->flg_error = true;
	state->error_msg = "Malloc failed";
	free_command(command);
}
