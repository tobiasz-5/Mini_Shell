/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_and_dollar.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 18:34:21 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/26 09:41:36 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
gestisce `$` e le var
crea una nuova struct-> env_var
->va oltre $, trova la variabile e la assegna a env_var,
se esiste inizializza il campo value della nuova struttura env_var
altrimenti lo imposta come stringa vuota
--> aggiunge var_value come argomento al comando,
imposta la flag booleana per indicare che c'era il dollar e
passa al token successivo
altrimenti se dopo $ non c'e un token_variable aggiunge
semplicemente $ come argomento
*/
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

/*
aggiunge una lista di redir al comando,
se non c'e niente in testa, redirection e' la prima redir altrimenti le 
aggiunge via via
*/
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
	redirection->next = NULL;
	return (1);
}

/*
crea effettivamente la struttura per la redir,
assegna il tipo del token come tipo di redir
dopo aver creato la redir va oltre di un token
ritorna la struttura
*/
t_redirection	*create_redirection(t_parser_state *state)
{
	t_redirection	*redirection;

	redirection = malloc(sizeof(t_redirection));
	if (!redirection)
		return (0);
	ft_memset(redirection, 0, sizeof(t_redirection));
	redirection->type = state->current_token->type;
	state->current_token = state->current_token->next;
	return (redirection);
}

/*
processa nome file associato alla redir
copia nome token come filename, assegna flag bool virgolette,
passa al token successivo
*/
int	parse_redirection_filename(t_parser_state *state,
			t_redirection *redirection)
{
	if (state->current_token && state->current_token->type == TOKEN_WORD)
	{
		redirection->filename = ft_strdup(state->current_token->token);
		redirection->is_quoted = state->current_token->single_quote
			|| state->current_token->double_quote;
		if (!redirection->filename)
			return (0);
		state->current_token = state->current_token->next;
		return (1);
	}
	else
		return (0);
}

/*
analizza redirezioni:
crea una struttura per la redir, chiama parse_redirection_filename,
aggiunge la redir al comando, restituisce 1 ok 0 fail
*/
int	parse_redirection(t_parser_state *state, t_command *command)
{
	t_redirection	*redirection;

	redirection = create_redirection(state);
	if (!redirection)
		return (0);
	if (!parse_redirection_filename(state, redirection))
	{
		free(redirection);
		return (0);
	}
	return (add_redirection_to_command(redirection, command));
}
