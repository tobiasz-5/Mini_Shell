/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer5_handle_token_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:46:21 by tschetti          #+#    #+#             */
/*   Updated: 2024/10/14 16:14:21 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

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
