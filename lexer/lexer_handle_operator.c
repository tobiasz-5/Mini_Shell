/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handle_operator.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 15:43:25 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/25 15:52:25 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
gestisce operatore doppio >>
->copia stringa due caratteri dall input,
crea il nodo, libera
*/
int	handle_double_operator(char *input, t_token_list *token_list)
{
	char	*operator_str;

	operator_str = ft_strndup(input + token_list->i, 2);
	if (!operator_str)
		return (-1);
	if (create_operator_node(operator_str, 2, token_list) == -1)
	{
		free(operator_str);
		return (-1);
	}
	free(operator_str);
	return (0);
}

/*
stessa logica ma per un singolo op >, <
*/
int	handle_single_operator(char *input, t_token_list *token_list)
{
	char	*operator_str;

	operator_str = ft_strndup(input + token_list->i, 1);
	if (!operator_str)
		return (-1);
	if (create_operator_node(operator_str, 1, token_list) == -1)
	{
		free(operator_str);
		return (-1);
	}
	free(operator_str);
	return (0);
}

/*
gestisce operatori,
determina se doppio o singolo -> due flussi diversi
*/
int	handle_operator_token(char *input, t_token_list *token_list)
{
	char	current_char;
	char	next_char;

	current_char = input[token_list->i];
	next_char = input[token_list->i + 1];
	if ((current_char == '>' && next_char == '>')
		|| (current_char == '<' && next_char == '<'))
	{
		if (handle_double_operator(input, token_list) == -1)
			return (-1);
	}
	else
	{
		if (handle_single_operator(input, token_list) == -1)
			return (-1);
	}
	return (token_list->i);
}
