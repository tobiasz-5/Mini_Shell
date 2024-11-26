/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_args_and_redir.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 18:34:30 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/26 12:47:42 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
aggiunge argomento alla lista di argomenti
t_list_args
-> alloca una struttura per il nuovo argomento
imposta il valore dell'argomento e lo stato bool delle quotes
-> crea la lista, collega i nodi
*/
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

/*
gestisce token word, duplica argomento,
se il token ha le double quotes, se ci sono var, le espande
->aggiunge argomento alla lista degli arg t_list_args -> campo arg,
va al token successivo
*/
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

/*
gestisce redirezioni,
ritorna 0 se parse_redirection fallisce 1 altrimenti
*/
int	handle_redirection_tokens(t_parser_state *state, t_command *command)
{
	if (!parse_redirection(state, command))
		return (0);
	return (1);
}

/*
gestisce token inaspettati/errori
*/
int	handle_unexpected_token(t_parser_state *state)
{
	state->flg_error = true;
	state->error_msg = "Unexpected token";
	return (0);
}

/*
processa argomenti e redir per ogni comando
->scorre token finche ci sono o finche trova una pipe
->se il corrente e' un word, lo gestisce, ritorna 0
se e' un dollar, gestisce, ritorna 0
se e' una redir, anche, altrimenti gestisce token errore
altrimenti ritorna 1 per liberare risorse allocate
*/
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
