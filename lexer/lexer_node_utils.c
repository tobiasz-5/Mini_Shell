/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer7_node_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: negambar <negambar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:46:24 by girindi          #+#    #+#             */
/*   Updated: 2024/11/05 16:16:57 by negambar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

void	add_token_to_list(t_token_list *token_list, char *token_str)
{
	t_token_node	*new_node;

	new_node = create_token_node(token_str,
			token_list->state->single_quote,
			token_list->state->double_quote);
	if (new_node)
		add_token_node(token_list, new_node);
}

void	add_token_node(t_token_list *token_list, t_token_node *new_node)
{
	if (!*(token_list->head))
		*(token_list->head) = new_node;
	else
		(*(token_list->tail))->next = new_node;
	*(token_list->tail) = new_node;
}

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
