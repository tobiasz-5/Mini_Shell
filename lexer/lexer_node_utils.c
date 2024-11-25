/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_node_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 14:16:05 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/25 14:38:14 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
aggiunge token alla lista(aggiunge nodo ->info)
con le info sulle quotes 
*/
void	add_token_to_list(t_token_list *token_list, char *token_str)
{
	t_token_node	*new_node;

	new_node = create_token_node(token_str,
			token_list->state->single_quote,
			token_list->state->double_quote);
	if (new_node)
		add_token_node(token_list, new_node);
}

/*
Aggiunge effettivamente token-nodi alla lista
->se la lista e' vuota new_node e' la testa,
->altimenti ogni nodo aggiunto via via diventa l ultimo
(tail pointer optimization in teoria)
*/
void	add_token_node(t_token_list *token_list, t_token_node *new_node)
{
	if (!*(token_list->head))
		*(token_list->head) = new_node;
	else
		(*(token_list->tail))->next = new_node;
	*(token_list->tail) = new_node;
}

/*
viene chiamata da process_quoted_token-> creiamo stringhe temporane
per gestire concatenazione testo tra virgolette o sintassi strana
come p'wd' o l's'
-> togliamo virgolette
nella fase del lexer, salviamo stato quotato/non quotato come bool)
*/
int	append_to_temp_token_str(t_token_state *state, char *temp_token)
{
	char	*new_str;

	new_str = ft_strjoin(state->token_str, temp_token);
	free(state->token_str);
	if (!new_str)
		return (-1);
	state->token_str = new_str;
	return (0);
}
