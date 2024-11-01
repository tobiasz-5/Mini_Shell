/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_minishell.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:46:29 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/01 12:39:42 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

void	execute_commands(t_command *command_list, t_shell_state *shell_state)
{
	if (!command_list)
		return ;
	if (process_all_heredocs(command_list, shell_state) != 0)
	{
		free_command_list(command_list);		
		return ;
	}
	if (command_list->next)
		execute_pipeline(command_list, shell_state);
	else
		execute_single_command(command_list, command_list, shell_state);
}

void	process_input(char *input, t_shell_state *shell_state)
{
	t_token_node	*tokens;
	t_command		*command_list;

	tokens = NULL;
	command_list = NULL;
	if (is_only_white_spaces(input))
		return ;
	tokens = lexer(input, shell_state);
	if (!tokens)
	{
		write(STDERR_FILENO, "Error tokenizing input\n", 23);
		return ;
	}
	command_list = parse_tokens(tokens, shell_state);
	free_token_list(tokens);
	if (!command_list)
	{
		write(STDERR_FILENO, "Error parsing tokens\n", 21);
		return ;
	}
	execute_commands(command_list, shell_state);
	free_command_list(command_list);
}
