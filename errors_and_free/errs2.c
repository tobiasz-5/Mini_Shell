/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errs2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 10:49:26 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/28 10:49:28 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

void	init_vars_for_expctd_cmdname(t_parser_state *state, t_command *command)
{
	state->flg_error = true;
	state->error_msg = "expected command name";
	free_command(command);
}

void	init_vars_for_error_adding_arg(t_parser_state *state,
					t_command *command)
{
	state->flg_error = true;
	state->error_msg = "Malloc failed";
	free_command(command);
}

void	init_var_failed_add_arg(t_parser_state *state)
{
	state->flg_error = true;
	state->error_msg = "Malloc failed";
}

void	init_var_for_arg_value(char *arg_value, t_list_args *new_arg)
{
	new_arg->arg = arg_value;
	new_arg->next = NULL;
}

void	init_var_for_malloc_rdrctn_err(t_parser_state *state)
{
	state->flg_error = true;
	state->error_msg = "Malloc failed for redirection";
}
