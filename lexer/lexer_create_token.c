/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_create_token.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 16:01:21 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/25 16:24:26 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
Crea nodo per token-> imposta i vari campi
-> Copia stringa token_str nel nodo
-> salva stato virgolette 
-> assegna tipo di token -> determine_token_type
-> puntatore nodo successivo via via NULL
-> Ritorna il nodo impostato
o NULL in caso di errore
*/
t_token_node	*create_token_node(char *token_str, bool single_quote,
					bool double_quote)
{
	t_token_node	*token_node;

	token_node = malloc(sizeof(t_token_node));
	if (!token_node)
		return (NULL);
	token_node->token = ft_strdup(token_str);
	if (!token_node->token)
	{
		free(token_node);
		return (NULL);
	}
	token_node->single_quote = single_quote;
	token_node->double_quote = double_quote;
	token_node->type = determine_token_type(token_node);
	token_node->next = NULL;
	return (token_node);
}

/*
Crea token dopo una serie di operazioni (rimozione virgolette,
espansioni, ecc..) a partire da cio che e' diventato alla fine
token_list->state->token_str, poi
-> crea nodo con la solita funzione
-> impostando i campi bool per le quotes
-> Aggiunge alla lista
-> reimposta a NULL token_str e le bool flag per le quotes
a false 
*/
void	create_real_token(t_token_list *token_list)
{
	t_token_node	*new_node;

	new_node = create_token_node(token_list->state->token_str,
			token_list->state->single_quote,
			token_list->state->double_quote);
	if (new_node)
	{
		add_token_node(token_list, new_node);
	}
	free(token_list->state->token_str);
	token_list->state->token_str = NULL;
	token_list->state->single_quote = false;
	token_list->state->double_quote = false;
}
