/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:46:06 by tschetti          #+#    #+#             */
/*   Updated: 2024/10/06 22:46:07 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

void	free_args_array(char **args_array)
{
	int	i;

	if (!args_array)
		return ;
	i = 0;
	while (args_array[i])
	{
		free(args_array[i]);
		i++;
	}
	free(args_array);
}

void	free_tokens(t_token_node *tokens)
{
	t_token_node	*temp;

	while (tokens)
	{
		temp = tokens;
		tokens = tokens->next;
		if (temp->token)
			free(temp->token);
		free(temp);
	}
}

void	free_token_list(t_token_node *tokens)
{
	t_token_node	*current;
	t_token_node	*next;

	current = tokens;
	while (current != NULL)
	{
		next = current->next;
		if (current->token != NULL)
			free(current->token);
		free(current);
		current = next;
	}
}
