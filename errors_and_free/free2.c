/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 10:49:06 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/28 10:50:49 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

void	free_command_and_args(t_command *command)
{
	t_list_args	*current_arg;
	t_list_args	*next_arg;

	if (command->cmd_name)
	{
		free(command->cmd_name);
		command->cmd_name = NULL;
	}
	current_arg = command->args_list;
	while (current_arg)
	{
		next_arg = current_arg->next;
		if (current_arg->arg)
		{
			free(current_arg->arg);
			current_arg->arg = NULL;
		}
		free(current_arg);
		current_arg = next_arg;
	}
	free(command);
}

void	free_command_redirections(t_redirection *redirections)
{
	t_redirection	*current_redirection;
	t_redirection	*next_redirection;

	current_redirection = redirections;
	while (current_redirection)
	{
		next_redirection = current_redirection->next;
		if (current_redirection->filename)
			free(current_redirection->filename);
		if (current_redirection->heredoc_filename)
		{
			free(current_redirection->heredoc_filename);
			current_redirection->heredoc_filename = NULL;
		}
		free(current_redirection);
		current_redirection = next_redirection;
	}
}

void	free_command(t_command *command)
{
	if (command)
	{
		free_command_redirections(command->redirections);
		free_command_and_args(command);
	}
}

void	free_command_list(t_command *command_list)
{
	t_command	*current;
	t_command	*next;

	current = command_list;
	while (current)
	{
		next = current->next;
		free_command(current);
		current = next;
	}
}

void	free_redirections(t_redirection *redirection)
{
	t_redirection	*current;
	t_redirection	*next;

	current = redirection;
	while (current)
	{
		next = current->next;
		if (current->filename)
			free(current->filename);
		free(current);
		current = next;
	}
}
