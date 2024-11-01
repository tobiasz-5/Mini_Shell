/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor10_heredoc_expansion.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 01:12:23 by tschetti          #+#    #+#             */
/*   Updated: 2024/10/16 01:12:24 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

#define MAX_EXPANDED_LINE 4096

static char	*get_env_var(t_shell_state *shell_state, const char *var_name)
{
	t_env_var	*env;

	env = shell_state->env_list;
	while (env)
	{
		if (ft_strcmp(env->name, var_name) == 0)
		{
			return (env->value);
		}
		env = env->next;
	}
	return ("");
}

static size_t	extract_var_name(const char *input, size_t i, char *var_name)
{
	int	var_len;

	var_len = 0;
	while ((input[i] >= 'A' && input[i] <= 'Z')
		|| (input[i] >= 'a' && input[i] <= 'z')
		|| (input[i] >= '0' && input[i] <= '9')
		|| (input[i] == '_'))
	{
		if (var_len < 255)
			var_name[var_len++] = input[i];
		i++;
	}
	var_name[var_len] = '\0';
	return (i);
}

static size_t	expand_env_var(const char *input, size_t i,
				t_expand_state *state)
{
	char	var_name[256];
	char	*var_value;
	size_t	var_value_len;
	size_t	remaining;

	i = extract_var_name(input, i, var_name);
	var_value = get_env_var(state->shell_state, var_name);
	var_value_len = ft_strlen(var_value);
	if (state->j + var_value_len >= MAX_EXPANDED_LINE - 1)
	{
		remaining = MAX_EXPANDED_LINE - 1 - state->j;
		if (remaining > 0)
		{
			ft_memcpy(&(state->output[state->j]), var_value, remaining);
			state->j += remaining;
		}
	}
	else
	{
		ft_memcpy(&(state->output[state->j]), var_value, var_value_len);
		state->j += var_value_len;
	}
	return (i);
}

char	*expand_var_in_heredoc(const char *input, t_shell_state *shell_state)
{
	static char		output[MAX_EXPANDED_LINE];
	t_expand_state	state;

	state.len = ft_strlen(input);
	state.i = 0;
	state.j = 0;
	state.in_quotes = false;
	state.output = output;
	state.shell_state = shell_state;
	while (state.i < state.len && state.j < MAX_EXPANDED_LINE - 1)
	{
		if (input[state.i] == '\'' || input[state.i] == '"')
		{
			state.in_quotes = !state.in_quotes;
			state.output[state.j++] = input[state.i++];
			continue ;
		}
		if (input[state.i] == '$')
			state.i = expand_env_var(input, state.i + 1, &state);
		else
			state.output[state.j++] = input[state.i++];
	}
	state.output[state.j] = '\0';
	return (state.output);
}
