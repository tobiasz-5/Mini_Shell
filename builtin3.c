/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:45:25 by tschetti          #+#    #+#             */
/*   Updated: 2024/10/16 17:17:55 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

bool	is_valid_var_name(const char *name)
{
	size_t	i;

	if (!name || !name[0])
		return (false);
	if (!(ft_isalpha(name[0]) || name[0] == '_'))
		return (false);
	i = 1;
	while (name[i])
	{
		if (!(ft_isalnum(name[i]) || name[i] == '_'))
			return (false);
		i++;
	}
	return (true);
}

void	free_env_var(t_env_var *env_var)
{
	if (env_var)
	{
		free(env_var->name);
		free(env_var->value);
		free(env_var);
	}
}

void	unset_env_var(t_env_var **env_list, const char *name)
{
	t_env_var	*current;
	t_env_var	*prev;

	if (env_list == NULL || *env_list == NULL || name == NULL)
		return ;
	current = *env_list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env_list = current->next;
			free_env_var(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	print_env_vars(t_env_var *env_list)
{
	while (env_list)
	{
		if (env_list->value != NULL && env_list->value[0] != '\0')
			printf("%s=%s\n", env_list->name, env_list->value);
		env_list = env_list->next;
	}
}

void	builtin_env(t_shell_state *shell_state)
{
	print_env_vars(shell_state->env_list);
}
