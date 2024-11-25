/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handle_token.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 14:41:43 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/25 15:23:47 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
"Crea" token TOKEN_VARIABLE (cio che segue $)
a partire da sottostringa -> token_list->i - start
dell'input.
contrassegna come TOKEN_VARIABLE 
*/
int	create_and_add_variable_token(char *input, int start,
				t_token_list *token_list)
{
	int				var_length;
	char			*var_name;
	t_token_node	*var_node;

	var_length = token_list->i - start;
	var_name = ft_strndup(input + start, var_length);
	if (!var_name)
		return (-1);
	var_node = create_token_node(var_name,
			token_list->state->single_quote,
			token_list->state->double_quote);
	free(var_name);
	if (!var_node)
		return (-1);
	var_node->type = TOKEN_VARIABLE;
	add_token_node(token_list, var_node);
	return (0);
}

/*
Gestisce token variabile 
-> trova len nome
-> funzione di supporto per creazione ->create_and_add...
-> Restituisce posizione aggiornata
*/
int	handle_variable_token(char *input, t_token_list *token_list)
{
	int	start;

	start = token_list->i;
	while (input[token_list->i] && (isalnum(input[token_list->i])
			|| input[token_list->i] == '_'))
		token_list->i++;
	if (create_and_add_variable_token(input, start, token_list) < 0)
		return (-1);
	return (token_list->i);
}

/*
gestisce token $?
exit status
-> trasforma l'intero in stringa -> itoa
-> crea nodo per valore exit status
-> aggiorna suo tipo -> WORD
-> aggiorna puntatore/cursore -> +2 (salta '$''?')
-> aggiunge il nodo alla lista
*/
int	handle_exit_status_token(t_token_list *token_list,
				t_shell_state *shell_state)
{
	char			*status_str;
	t_token_node	*status_node;

	status_str = ft_itoa(shell_state->last_exit_status);
	if (!status_str)
		return (-1);
	status_node = create_token_node(status_str,
			token_list->state->single_quote,
			token_list->state->double_quote);
	free(status_str);
	if (!status_node)
		return (-1);
	status_node->type = TOKEN_WORD;
	add_token_node(token_list, status_node);
	token_list->i += 2;
	return (token_list->i);
}

/*
gestisce token che inizia con $
-> se subito dopo abbiamo ? -> gestisco exit_status
altrimenti:
creo nodo per il token dollar
->gli assegno il suo tipo -> TOKEN_DOLLAR
->va oltre '$'
->analizza se la sintassi del name e' valida [isalnum,_]
->gestisce il token_variable
->ritorna posizione/puntatore/cursore aggiornato
*/
int	handle_dollar_token(char *input, t_token_list *token_list,
			t_shell_state *shell_state)
{
	char			*dollar_str;
	t_token_node	*dollar_node;

	if (input[token_list->i + 1] == '?')
		return (handle_exit_status_token(token_list, shell_state));
	dollar_str = ft_strndup("$", 1);
	if (!dollar_str)
		return (-1);
	dollar_node = create_token_node(dollar_str,
			token_list->state->single_quote, token_list->state->double_quote);
	free(dollar_str);
	if (!dollar_node)
		return (-1);
	dollar_node->type = TOKEN_DOLLAR;
	add_token_node(token_list, dollar_node);
	token_list->i++;
	if (input[token_list->i] && (isalnum(input[token_list->i])
			|| input[token_list->i] == '_'))
		return (handle_variable_token(input, token_list));
	return (token_list->i);
}
