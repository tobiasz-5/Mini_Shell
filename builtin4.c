/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:45:30 by tschetti          #+#    #+#             */
/*   Updated: 2024/10/25 00:27:58 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

t_env_var	*find_env_var(t_env_var *env_list, const char *name)
{
	t_env_var	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

int	is_builtin(char *cmd_name)
{
	if (ft_strcmp(cmd_name, "cd") == 0
		|| ft_strcmp(cmd_name, "pwd") == 0
		|| ft_strcmp(cmd_name, "export") == 0
		|| ft_strcmp(cmd_name, "unset") == 0
		|| ft_strcmp(cmd_name, "env") == 0
		|| ft_strcmp(cmd_name, "exit") == 0
		|| ft_strcmp(cmd_name, "echo") == 0)
		return (1);
	return (0);
}
