/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:46:41 by tschetti          #+#    #+#             */
/*   Updated: 2024/10/30 12:27:53 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

bool	set_command_name(t_parser_state *state, t_command *command)
{
	if (state->current_token->type == TOKEN_WORD)
	{
		command->cmd_name = ft_strdup(state->current_token->token);
		if (!command->cmd_name)
		{
			init_vars_for_cmdname_error(state, command);
			return (false);
		}
		state->current_token = state->current_token->next;
	}
	else
	{
		command->cmd_name = ft_strdup("/bin/true");
		if (!command->cmd_name)
		{
			init_vars_for_cmdname_error(state, command);
			return (false);
		}
	}
	return (true);
}

t_command	*parse_command(t_parser_state *state,
			t_shell_state *shell_state)
{
	t_command	*command;

	command = malloc(sizeof(t_command));
	if (!command)
		return (init_vars_for_not_cmd(state), NULL);
	ft_memset(command, 0, sizeof(t_command));
	init_vars_for_parsing_command(command);
	if (state->current_token && (state->current_token->type == TOKEN_WORD
			|| is_redirection_token(state->current_token->type)))
	{
		if (!set_command_name(state, command))
			return (NULL);
	}
	else
		return (init_vars_for_expctd_cmdname(state, command), NULL);
	if (!add_argument(command, ft_strdup(command->cmd_name), false, false))
		return (init_vars_for_error_adding_arg(state, command), NULL);
	if (!parse_arguments_and_redirection(state, command, shell_state))
		return (free_command(command), NULL);
	return (command);
}

t_command	*parse_tokens(t_token_node *tokens, t_shell_state *shell_state)
{
	t_parser_state	state;
	t_command		*cmd_list;
	t_command		*last_command;
	t_command		*command;

	cmd_list = NULL;
	last_command = NULL;
	init_var_for_parsing_tokens(tokens, &state, &cmd_list, &last_command);
	while (state.current_token && !state.flg_error)
	{
		command = parse_command(&state, shell_state);
		if (state.flg_error)
		{
			call_error_and_free(&state, &cmd_list);
			return (NULL);
		}
		if (!cmd_list)
			cmd_list = command;
		else
			last_command->next = command;
		last_command = command;
		if (state.current_token && state.current_token->type == TOKEN_PIPE)
			state.current_token = state.current_token->next;
	}
	return (cmd_list);
}
