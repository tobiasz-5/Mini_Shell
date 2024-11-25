/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_variable.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 15:40:45 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/23 16:41:39 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

// t_env_var	*find_local_env_var(t_env_var *env_list, const char *name)
// {
// 	t_env_var	*current;

// 	current = env_list;
// 	while (current)
// 	{
// 		if (ft_strcmp(current->name, name) == 0)
// 			return (current);
// 		current = current->next;
// 	}
// 	return (NULL);
// }

/*
trova valore di una variabile e lo copia nella destinazione
*/
int	handle_variable_value(char **dest_ptr, const char **cursor,
				t_shell_state *shell_state, int var_len)
{
	char		*extracted_var_name;
	char		*var_value;
	t_env_var	*local_var;
	int			value_len;

	extracted_var_name = ft_strndup(*cursor, var_len);
	if (!extracted_var_name)
		return (-1);
	local_var = find_env_var(shell_state->env_list, extracted_var_name);
	if (local_var)
		var_value = local_var->value;
	else
		var_value = "";
	free(extracted_var_name);
	if (var_value)
	{
		value_len = ft_strlen(var_value);
		ft_memcpy(*dest_ptr, var_value, value_len);
		*dest_ptr += value_len;
	}
	*cursor += var_len;
	return (0);
}

/*
trova len nome variabile, determina se e' il token e' una var valida->
gestisce l espansione tramite handle_variable_value
*/
int	expand_variable(char **dest_ptr, const char **cursor,
				t_shell_state *shell_state)
{
	int	var_len;

	var_len = get_variable_name_length(*cursor);
	if (var_len > 0)
	{
		if (handle_variable_value(dest_ptr, cursor, shell_state, var_len) == -1)
			return (-1);
	}
	else
	{
		add_dollar_char(dest_ptr);
	}
	return (0);
}
