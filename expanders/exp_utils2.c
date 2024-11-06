/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander5_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: negambar <negambar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:46:05 by girindi          #+#    #+#             */
/*   Updated: 2024/11/05 16:16:39 by negambar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

void	add_dollar_char(char **dest_ptr)
{
	**dest_ptr = '$';
	(*dest_ptr)++;
}

int	copy_literal_text(char **dest_ptr, const char *start, const char *end)
{
	int	len;

	len = end - start;
	ft_memcpy(*dest_ptr, start, len);
	*dest_ptr += len;
	return (0);
}

int	get_variable_name_length(const char *str)
{
	int	var_len;

	var_len = 0;
	while (isalnum(str[var_len]) || str[var_len] == '_')
		var_len++;
	return (var_len);
}

int	get_variable_value_length(const char *var_name, int var_len,
					t_shell_state *shell_state)
{
	char		*extracted_var_name;
	char		*var_value;
	int			value_len;
	t_env_var	*local_var;

	extracted_var_name = ft_strndup(var_name, var_len);
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
		return (value_len);
	}
	return (0);
}
