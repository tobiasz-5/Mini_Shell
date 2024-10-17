/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser4_redir_and_dollar.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:46:49 by tschetti          #+#    #+#             */
/*   Updated: 2024/10/15 20:08:02 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

int	handle_dollar_and_variable(t_parser_state *state,
				t_command *command, t_shell_state *shell_state)
{
	char		*var_name;
	char		*var_value;
	t_env_var	*env_var;

	state->current_token = state->current_token->next;
	if (state->current_token && state->current_token->type == TOKEN_VARIABLE)
	{
		var_name = state->current_token->token;
		env_var = find_env_var(shell_state->env_list, var_name);
		if (env_var)
			var_value = env_var->value;
		else
			var_value = "";
		if (!add_argument(command, ft_strdup(var_value), false, false))
			return (init_var_failed_add_arg(state), 0);
		command->has_dollar = true;
		state->current_token = state->current_token->next;
	}
	else
	{
		if (!add_argument(command, ft_strdup("$"), false, false))
			return (init_var_failed_add_arg(state), 0);
	}
	return (1);
}

int	add_redirection_to_command(t_redirection *redirection, t_command *command)
{
	t_redirection	*last;

	if (!command->redirections)
		command->redirections = redirection;
	else
	{
		last = command->redirections;
		while (last->next)
			last = last->next;
		last->next = redirection;
	}
	return (1);
}

int	parse_redirection(t_parser_state *state, t_command *command)
{
	t_redirection	*redirection;

	redirection = malloc(sizeof(t_redirection));
	if (!redirection)
		return (0);
	ft_memset(redirection, 0, sizeof(t_redirection));
	redirection->type = state->current_token->type;
	state->current_token = state->current_token->next;
	if (state->current_token && state->current_token->type == TOKEN_WORD)
	{
		redirection->filename = ft_strdup(state->current_token->token);
		redirection->is_quoted = state->current_token->single_quote
			|| state->current_token->double_quote;
		state->current_token = state->current_token->next;
	}
	else
	{
		free(redirection);
		return (0);
	}
	return (add_redirection_to_command(redirection, command));
}
