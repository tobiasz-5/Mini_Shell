/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:46:58 by tschetti          #+#    #+#             */
/*   Updated: 2024/10/14 16:29:42 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

int	find_closing_quote(char *input, int i, char quote)
{
	while (input[i] && input[i] != quote)
		i++;
	if (!input[i])
		return (-1);
	return (i);
}

void	handle_unclosed_quote_error(t_token_state *state)
{
	printf("Error: Unclosed quote detected\n");
	if (state->token_str)
	{
		free(state->token_str);
		state->token_str = NULL;
	}
}

void	set_quote_flag(t_token_state *state, char quote)
{
	if (quote == '\'')
		state->single_quote = true;
	else if (quote == '"')
		state->double_quote = true;
}

int	handle_token_error(t_token_state *state)
{
	if (state->token_str)
	{
		free(state->token_str);
		state->token_str = NULL;
	}
	return (-1);
}

t_token_type	determine_token_type(t_token_node *token_node)
{
	char	*token_str;

	if (token_node->single_quote || token_node->double_quote)
	{
		return (TOKEN_WORD);
	}
	token_str = token_node->token;
	if (strcmp(token_str, "|") == 0)
		return (TOKEN_PIPE);
	else if (strcmp(token_str, ">>") == 0)
		return (TOKEN_REDIR_APPEND);
	else if (strcmp(token_str, "<<") == 0)
		return (TOKEN_HEREDOC);
	else if (strcmp(token_str, "<") == 0)
		return (TOKEN_REDIR_IN);
	else if (strcmp(token_str, ">") == 0)
		return (TOKEN_REDIR_OUT);
	else if (strcmp(token_str, "$") == 0)
		return (TOKEN_DOLLAR);
	else
		return (TOKEN_WORD);
}
