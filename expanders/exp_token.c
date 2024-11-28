/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 10:52:52 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/28 10:52:53 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
Costruisce stringa espansa sostituendo variabili presenti in input
*/
int	build_expanded_string(const char *token, t_var_expand *v_exp,
				t_shell_state *shell_state)
{
	const char	*cursor;
	const char	*dollar_sign;
	char		*dest_ptr;
	int			status;

	cursor = token;
	dest_ptr = v_exp->result;
	dollar_sign = ft_strchr(cursor, '$');
	while (dollar_sign)
	{
		status = copy_literal_text(&dest_ptr, cursor, dollar_sign);
		if (status == -1)
			return (-1);
		cursor = dollar_sign + 1;
		if (*cursor == '\0')
			break ;
		status = expand_variable(&dest_ptr, &cursor, shell_state);
		if (status == -1)
			return (-1);
		dollar_sign = ft_strchr(cursor, '$');
	}
	ft_strcpy(dest_ptr, cursor);
	return (0);
}

/*
Gestisce len per espansione di una var, aggiorna anche il cursore/puntatore
*/
int	handle_variable_expansion(const char **cursor, int length,
				t_var_expand *v_exp, t_shell_state *shell_state)
{
	int	var_value_len;

	var_value_len = get_variable_value_length(*cursor, length, shell_state);
	if (var_value_len == -1)
		return (-1);
	v_exp->total_length += var_value_len;
	*cursor += length;
	return (0);
}

/*
calcola len token considerata espansione
*/
int	calculate_total_length(const char *token, t_var_expand *v_exp,
				t_shell_state *shell_state)
{
	const char	*cursor;
	const char	*dollar_sign;
	int			length;

	cursor = token;
	v_exp->total_length = 0;
	dollar_sign = ft_strchr(cursor, '$');
	while (dollar_sign != NULL)
	{
		length = dollar_sign - cursor;
		v_exp->total_length += length;
		cursor = dollar_sign + 1;
		dollar_sign = ft_strchr(cursor, '$');
		length = get_variable_name_length(cursor);
		if (length > 0)
		{
			if (handle_variable_expansion(&cursor, length,
					v_exp, shell_state) == -1)
				return (-1);
		}
		else
			v_exp->total_length++;
	}
	v_exp->total_length += ft_strlen(cursor);
	return (0);
}

/*
Espande tutte le variabili dell input, tramite build_expanded_string
e restituisce la stringa completa
*/
char	*expand_variable_in_token(const char *token, t_var_expand *v_exp,
					t_shell_state *shell_state)
{
	int	status;

	if (!token || !v_exp || !shell_state)
		return (NULL);
	status = calculate_total_length(token, v_exp, shell_state);
	if (status == -1)
		return (NULL);
	v_exp->result = malloc(v_exp->total_length + 1);
	if (!v_exp->result)
		return (NULL);
	status = build_expanded_string(token, v_exp, shell_state);
	if (status == -1)
	{
		free(v_exp->result);
		return (NULL);
	}
	return (v_exp->result);
}
