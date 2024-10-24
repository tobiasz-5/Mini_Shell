/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer3_special_process.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:46:17 by tschetti          #+#    #+#             */
/*   Updated: 2024/10/16 17:06:58 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

int	handle_expanded_token(char **temp_token, char quote,
				t_shell_state *shell_state)
{
	char	*expanded_token;

	if (quote == '"')
	{
		expanded_token = expand_var_in_string(*temp_token, shell_state);
		free(*temp_token);
		if (!expanded_token)
			return (-1);
		*temp_token = expanded_token;
	}
	return (0);
}

int	process_quoted_token(char *input, char quote,
			t_token_list *token_list, t_shell_state *shell_state)
{
	int		start;
	int		end;
	char	*temp_token;
	int		result;

	token_list->i++;
	start = token_list->i;
	end = find_closing_quote(input, token_list->i, quote);
	if (end == -1)
	{
		handle_unclosed_quote_error(token_list->state);
		return (-1);
	}
	temp_token = ft_strndup(input + start, end - start);
	if (!temp_token)
		return (-1);
	if (handle_expanded_token(&temp_token, quote, shell_state) == -1)
		return (-1);
	result = append_to_temp_token_str(token_list->state, temp_token);
	free(temp_token);
	if (result == -1)
		return (-1);
	set_quote_flag(token_list->state, quote);
	token_list->i = end + 1;
	return (token_list->i);
}

int	process_special_token(char *input, t_token_list *token_list,
				t_shell_state *shell_state)
{
	char	current_char;

	current_char = input[token_list->i];
	if (current_char == '$')
		return (handle_dollar_token(input, token_list, shell_state));
	else
		return (handle_operator_token(input, token_list));
}

// int	process_quoted_token(char *input, char quote,
// 			t_token_list *token_list, t_shell_state *shell_state)
// {
// 	int		start;
// 	int		end;
// 	char	*temp_token;
// 	char	*expanded_token;
// 	int		result;

// 	token_list->i++;
// 	start = token_list->i;
// 	end = find_closing_quote(input, token_list->i, quote);
// 	if (end == -1)
// 	{
// 		handle_unclosed_quote_error(token_list->state);
// 		return (-1);
// 	}
// 	temp_token = ft_strndup(input + start, end - start);
// 	if (!temp_token)
// 		return (-1);
// 	if (quote == '"')
// 	{
// 		expanded_token = expand_var_in_string(temp_token, shell_state);
// 		free(temp_token);
// 		if (!expanded_token)
// 			return (-1);
// 		temp_token = expanded_token;
// 	}
// 	result = append_to_temp_token_str(token_list->state, temp_token);
// 	free(temp_token);
// 	if (result == -1)
// 		return (-1);
// 	set_quote_flag(token_list->state, quote);
// 	token_list->i = end + 1;
// 	return (token_list->i);
// }