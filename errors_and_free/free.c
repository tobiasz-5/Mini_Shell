/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 10:49:13 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/28 10:49:15 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

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

void	free_all(t_command *all_cmds, t_fork_info *finfo,
			t_shell_state *shell_state)
{
	free_args_array(finfo->args_array);
	free(finfo->args_quote_flags);
	free_command_list(all_cmds);
	clean_shell_state(shell_state);
	exit(shell_state->last_exit_status);
}
