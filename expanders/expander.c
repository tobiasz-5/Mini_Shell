/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: negambar <negambar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:45:58 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/05 16:27:25 by negambar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

void	process_input_string(char *str, char *result, char *exit_status_str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] == '?')
		{
			copy_exit_status(result, &j, exit_status_str);
			i += 2;
		}
		else
		{
			result[j] = str[i];
			j++;
			i++;
		}
	}
	result[j] = '\0';
}

int	i_ex(char **result, int expanded_length,
			char **exit_status_str, t_shell_state *shell_state)
{
	*result = malloc(expanded_length + 1);
	if (!*result)
		return (-1);
	*exit_status_str = ft_itoa(shell_state->last_exit_status);
	if (!*exit_status_str)
	{
		free(*result);
		return (-1);
	}
	return (0);
}

static int	calculate_expanded_length(char *str, t_shell_state *shell_state)
{
	int		len;
	int		i;
	int		exit_len;
	char	*exit_status_str;

	init_dasvidaniya_norminette(&len, &i);
	exit_status_str = ft_itoa(shell_state->last_exit_status);
	if (!exit_status_str)
		return (-1);
	exit_len = ft_strlen(exit_status_str);
	free(exit_status_str);
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] == '?')
		{
			len += exit_len;
			i += 2;
		}
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

char	*expand_var_in_string(char *str, t_shell_state *shell_state)
{
	char	*result;
	int		expanded_length;
	char	*exit_status_str;
	int		init_result;

	if (!str)
		return (NULL);
	expanded_length = calculate_expanded_length(str, shell_state);
	if (expanded_length == -1)
		return (NULL);
	init_result = i_ex(&result, expanded_length, &exit_status_str, shell_state);
	if (init_result == -1)
		return (NULL);
	process_input_string(str, result, exit_status_str);
	free(exit_status_str);
	return (result);
}

void	expand_exit_status(t_token_node *tokens, t_shell_state *shell_state)
{
	t_token_node	*current;
	char			*expanded_token;

	current = tokens;
	while (current)
	{
		if (current->double_quote)
		{
			expanded_token = expand_var_in_string(current->token, shell_state);
			free(current->token);
			current->token = expanded_token;
		}
		current = current->next;
	}
}
