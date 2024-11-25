/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_process_token.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 14:16:37 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/25 14:16:39 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
processa token semplici(no quotes, no operators)
*/
int	process_normal_token(char *input, t_token_list *token_list)
{
	int		start;
	char	*temp_token;
	char	*new_str;

	start = token_list->i;
	while (input[token_list->i] && input[token_list->i] != ' '
		&& !is_operator(input[token_list->i])
		&& input[token_list->i] != '\''
		&& input[token_list->i] != '"')
		token_list->i++;
	temp_token = ft_strndup(input + start, token_list->i - start);
	if (!temp_token)
		return (-1);
	new_str = ft_strjoin(token_list->state->token_str, temp_token);
	free(token_list->state->token_str);
	free(temp_token);
	if (!new_str)
		return (-1);
	token_list->state->token_str = new_str;
	return (token_list->i);
}

/*
se c'e bisogno espande(e crea token), altrimenti processa 
token "normale"(semplice)
*/
int	handle_expansions_and_normal_tokens(char *input, t_token_list *token_list,
					t_shell_state *shell_state)
{
	t_var_expand	v_exp;
	char			*expanded_token;
	t_token_node	*expanded_node;

	expanded_token = NULL;
	(void)shell_state;
	if (input[token_list->i] == '$')
	{
		expanded_token = expand_variable_in_token(input, &v_exp, shell_state);
		if (expanded_token)
		{
			expanded_node = create_token_node(expanded_token,
					token_list->state->single_quote,
					token_list->state->double_quote);
			free(expanded_token);
			if (!expanded_node)
				return (-1);
			add_token_node(token_list, expanded_node);
		}
		if (v_exp.var_value == NULL)
			v_exp.var_value = "";
		token_list->i += ft_strlen(v_exp.var_value);
		return (0);
	}
	return (process_normal_token(input, token_list));
}

/*
processa token: che hanno redir, o quotati, o token normali/da espandere
*/
int	prcs_tokn(char *input, t_token_list *token_list, t_shell_state *shell_state)
{
	char	quote;

	if (is_operator(input[token_list->i]))
		return (process_special_token(input, token_list, shell_state));
	else if (input[token_list->i] == '\'' || input[token_list->i] == '"')
	{
		quote = input[token_list->i];
		return (process_quoted_token(input, quote, token_list, shell_state));
	}
	else
		return (handle_expansions_and_normal_tokens(input,
				token_list, shell_state));
}

/*
processa ogni token, li crea effettivamente alla fine
*/
int	prcs_nxt_tkn(char *input, t_token_list *token_list,
			t_shell_state *shell_state)
{
	if (is_operator(input[token_list->i]))
	{
		return (prcs_tokn(input, token_list, shell_state));
	}
	while (input[token_list->i] && input[token_list->i] != ' '
		&& !is_operator(input[token_list->i]))
	{
		token_list->i = prcs_tokn(input, token_list, shell_state);
		if (token_list->i == -1)
			return (handle_token_error(token_list->state));
	}
	if (token_list->state->token_str)
		create_real_token(token_list);
	return (token_list->i);
}
