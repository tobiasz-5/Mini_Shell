/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:45:23 by tschetti          #+#    #+#             */
/*   Updated: 2024/10/16 17:17:48 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

void	export_env_var(t_env_var **env_list, const char *name,
			const char *value)
{
	t_env_var	*var;
	t_env_var	*new_var;

	var = find_env_var(*env_list, name);
	if (var)
	{
		free(var->value);
		var->value = ft_strdup(value);
	}
	else
	{
		new_var = malloc(sizeof(t_env_var));
		if (!new_var)
			return ;
		new_var->name = ft_strdup(name);
		new_var->value = ft_strdup(value);
		new_var->next = *env_list;
		*env_list = new_var;
	}
}

void	process_export_value(char **var_value, bool double_quote,
				bool single_quote)
{
	size_t	len;
	char	*clean_value;

	if ((double_quote || single_quote) && var_value && *var_value)
	{
		if ((*var_value)[0] == '"' || (*var_value)[0] == '\'')
		{
			len = ft_strlen(*var_value);
			if ((*var_value)[len - 1] == '"' || (*var_value)[len - 1] == '\'')
			{
				clean_value = ft_strndup((*var_value) + 1, len - 2);
				if (clean_value)
				{
					free(*var_value);
					*var_value = clean_value;
				}
			}
		}
	}
}

void	export_env_var_empty(t_env_var **env_list, const char *name)
{
	t_env_var	*var;
	t_env_var	*new_var;

	var = find_env_var(*env_list, name);
	if (var)
	{
		var->exported_empty = true;
		if (var->value)
		{
			free(var->value);
		}
		var->value = ft_strdup("");
	}
	else
	{
		new_var = malloc(sizeof(t_env_var));
		if (!new_var)
			return ;
		new_var->name = ft_strdup(name);
		new_var->value = ft_strdup("");
		new_var->exported_empty = true;
		new_var->next = *env_list;
		*env_list = new_var;
	}
}

int	parse_export_input(const char *input, char **var_name, char **var_value)
{
	char	*equal_sign;

	equal_sign = ft_strchr(input, '=');
	if (equal_sign)
	{
		*var_name = ft_strndup(input, equal_sign - input);
		*var_value = ft_strdup(equal_sign + 1);
	}
	else
	{
		*var_name = ft_strdup(input);
		*var_value = NULL;
	}
	if (!(*var_name) || !*(*var_name) || !is_valid_var_name(*var_name))
	{
		printf("export: `%s`: not a valid identifier\n", input);
		free(*var_name);
		free(*var_value);
		return (-1);
	}
	return (0);
}

void	handle_export_command(t_env_var **env_list, const char *input,
				bool double_quote, bool single_quote)
{
	char	*var_name;
	char	*var_value;

	var_name = NULL;
	var_value = NULL;
	if (parse_export_input(input, &var_name, &var_value) != 0)
	{
		return ;
	}
	if (var_value == NULL)
	{
		export_env_var_empty(env_list, var_name);
	}
	else
	{
		process_export_value(&var_value, double_quote, single_quote);
		export_env_var(env_list, var_name, var_value);
	}
	free(var_name);
	free(var_value);
}

// void	handle_export_command(t_env_var **env_list, const char *input,
// 			bool double_quote, bool single_quote)
// {
// 	char	*equal_sign;
// 	char	*var_name;
// 	char	*var_value;

// 	equal_sign = ft_strchr(input, '=');
// 	if (equal_sign)
// 	{
// 		var_name = ft_strndup(input, equal_sign - input);
// 		var_value = ft_strdup(equal_sign + 1);
// 	}
// 	else
// 	{
// 		var_name = ft_strdup(input);
// 		var_value = NULL;
// 	}
// 	if (!var_name || !*var_name || !is_valid_var_name(var_name))
// 	{
// 		printf("export: `%s`: not a valid identifier\n", input);
// 		free(var_name);
// 		free(var_value);
// 		return ;
// 	}
// 	if (var_value == NULL)
// 		export_env_var_empty(env_list, var_name);
// 	else
// 	{
// 		if (var_value)
// 			process_export_value(&var_value, double_quote, single_quote);
// 		export_env_var(env_list, var_name, var_value);
// 	}
// 	free(var_name);
// 	free(var_value);
// }
