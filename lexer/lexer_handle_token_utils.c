/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handle_token_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 15:25:23 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/25 15:42:40 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
Crea nodo per operatore (<,>,>>)
-Prende stringa operator_str e incremento -> + 1 se
single operatore + 2 se double operator <, >>
-> salva stato virgolette
-> aggiunge nodo alla lista
-> avanza cursore di increment +1 o + 2
->ritorna posizione aggiornata cursore
*/
int	create_operator_node(char *operator_str, int increment,
				t_token_list *token_list)
{
	t_token_node	*op_node;

	op_node = create_token_node(operator_str,
			token_list->state->single_quote,
			token_list->state->double_quote);
	if (!op_node)
		return (-1);
	add_token_node(token_list, op_node);
	token_list->i += increment;
	return (token_list->i);
}
