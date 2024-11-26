/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 18:32:18 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/25 18:32:20 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
--->ritorna -1 in caso di errore -> quindi se non trova una
quote a chiudere
-se c'e una quote di apertura
deve essercene una di chiusura altrimenti errore
--->altrimenti ritorna posizione/indice della quote di chiusura
*/
int	find_closing_quote(char *input, int i, char quote)
{
	while (input[i] && input[i] != quote)
		i++;
	if (!input[i])
		return (-1);
	return (i);
}

/*
chiamante: process_quoted_token
gestisce stringhe senza quote di chiusura->
printa messaggio di errore,
libera memoria token_str,
reimposta a null [dangling pointer]
*/
void	handle_unclosed_quote_error(t_token_state *state)
{
	printf("Error: Unclosed quote detected\n");
	if (state->token_str)
	{
		free(state->token_str);
		state->token_str = NULL;
	}
}

/*
imposta la flag bool per le virgolette nella struct
token_state
*/
void	set_quote_flag(t_token_state *state, char quote)
{
	if (quote == '\'')
		state->single_quote = true;
	else if (quote == '"')
		state->double_quote = true;
}

/*
gestisce errori nei token, chiamata piu volte
in prcs_nxt_tkn,
-> libera stringa temporanea, reimposta a null token_str
*/
int	handle_token_error(t_token_state *state)
{
	if (state->token_str)
	{
		free(state->token_str);
		state->token_str = NULL;
	}
	return (-1);
}

/*
determina l'enum type del token
se tra virgolette e' sempre un word
altrimenti confronta token e
simbolo struttura enum t_token_type
tutti altri casi e' word
*/
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
