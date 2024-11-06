/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer6_create_token.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: negambar <negambar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:46:22 by girindi          #+#    #+#             */
/*   Updated: 2024/11/05 16:16:57 by negambar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

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
