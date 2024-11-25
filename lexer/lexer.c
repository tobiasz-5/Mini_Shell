/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 14:16:21 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/25 14:16:21 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
**head e tail per aver un riferimento sempre a testa e coda
->efficiente per op. su liste (es. inserimento)
*/
void	lexer_var_init(t_token_node **head, t_token_node **tail,
			t_token_state *state, t_token_list *token_list)
{
	*head = NULL;
	*tail = NULL;
	state->token_str = NULL;
	state->single_quote = false;
	state->double_quote = false;
	token_list->head = head;
	token_list->tail = tail;
	token_list->state = state;
	token_list->i = 0;
	token_list->result = 0;
}

/*
i->indice interno a t_token_list.scorre la stringa -> input[token_list.i]
*/
t_token_node	*lexer(char *input, t_shell_state *shell_state)
{
	t_token_node	*head;
	t_token_node	*tail;
	t_token_state	state;
	t_token_list	token_list;

	lexer_var_init(&head, &tail, &state, &token_list);
	while (input[token_list.i])
	{
		token_list.i = skip_spaces(input, token_list.i);
		token_list.result = prcs_nxt_tkn(input, &token_list, shell_state);
		if (token_list.result == -1)
		{
			if (head)
				free_tokens(head);
			return (NULL);
		}
		token_list.i = token_list.result;
		if (input[token_list.i] == ' ')
			token_list.i++;
	}
	return (head);
}
