/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 16:46:05 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/23 16:46:07 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
Aggiunge $ a dest e avanza il puntatore
*/
void	add_dollar_char(char **dest_ptr)
{
	**dest_ptr = '$';
	(*dest_ptr)++;
}

/*
Copia testo "letterale" (non var o simboli speciali) in dest
*/
int	copy_literal_text(char **dest_ptr, const char *start, const char *end)
{
	int	len;

	len = end - start;
	ft_memcpy(*dest_ptr, start, len);
	*dest_ptr += len;
	return (0);
}

/*
calcola len singolo nome di variabile-> USER->4
*/
int	get_variable_name_length(const char *str)
{
	int	var_len;

	var_len = 0;
	while (isalnum(str[var_len]) || str[var_len] == '_')
		var_len++;
	return (var_len);
}

/*
trova len valore variabile
*/
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
