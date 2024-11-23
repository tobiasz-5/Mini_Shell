/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2_args_and_redir.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: negambar <negambar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:46:45 by girindi          #+#    #+#             */
/*   Updated: 2024/11/05 16:27:25 by negambar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

int	add_argument(t_command *command, char *arg_value, bool single_quote,
				bool double_quote)
{
	t_list_args	*new_arg;
	t_list_args	*last;

	if (!arg_value)
		return (0);
	new_arg = malloc(sizeof(t_list_args));
	if (!new_arg)
	{
		free(arg_value);
		return (0);
	}
	new_arg->arg = arg_value;
	new_arg->single_quote = single_quote;
	new_arg->double_quote = double_quote;
	new_arg->next = NULL;
	if (!command->args_list)
		command->args_list = new_arg;
	else
	{
		last = command->args_list;
		while (last->next)
			last = last->next;
		last->next = new_arg;
	}
	return (1);
}

int	handle_token_word(t_parser_state *state, t_command *command,
		t_var_expand *v_exp, t_shell_state *shell_state)
{
	char	*arg_value;
	char	*expanded_value;

	arg_value = ft_strdup(state->current_token->token);
	if (!arg_value)
		return (init_var_failed_add_arg(state), 0);
	if (state->current_token->double_quote)
	{
		expanded_value = expand_variable_in_token(arg_value,
				v_exp, shell_state);
		free(arg_value);
		if (!expanded_value)
			return (init_var_failed_add_arg(state), 0);
		arg_value = expanded_value;
	}
	if (!add_argument(command, arg_value, state->current_token->single_quote,
			state->current_token->double_quote))
		return (init_var_failed_add_arg(state), 0);
	state->current_token = state->current_token->next;
	return (1);
}

int	handle_redirection_tokens(t_parser_state *state, t_command *command)
{
	if (!parse_redirection(state, command))
		return (0);
	return (1);
}

int	handle_unexpected_token(t_parser_state *state)
{
	state->flg_error = true;
	state->error_msg = "Unexpected token";
	return (0);
}

int	parse_arguments_and_redirection(t_parser_state *state, t_command *command,
					t_shell_state *shell_state)
{
	t_var_expand	v_exp;

	while (state->current_token && state->current_token->type != TOKEN_PIPE)
	{
		if (state->current_token->type == TOKEN_WORD)
		{
			if (!handle_token_word(state, command, &v_exp, shell_state))
				return (0);
		}
		else if (state->current_token->type == TOKEN_DOLLAR)
		{
			if (!handle_dollar_and_variable(state, command, shell_state))
				return (0);
		}
		else if (is_redirection_token(state->current_token->type))
		{
			if (!handle_redirection_tokens(state, command))
				return (0);
		}
		else
			return (handle_unexpected_token(state));
	}
	return (1);
}
