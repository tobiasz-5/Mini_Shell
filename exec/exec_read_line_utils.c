/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_read_line_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 19:24:02 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/27 10:05:51 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

#define MAX_LINE_LENGTH 4096

/*
legge un char alla volta da stdin usando read,
lo memorizza in buffer, read restituisce numero di byte letti
quindi 1 se ok, 0 se fine file, -1 se errore
->se non e' causato da un segnale(eintr) ma appunto da read chiama perror
*/
ssize_t	read_single_char(char *buffer)
{
	ssize_t	bytes_read;

	bytes_read = read(STDIN_FILENO, buffer, 1);
	if (bytes_read == -1 && errno != EINTR)
	{
		perror("read");
	}
	return (bytes_read);
}

/*
verifica se il char leto e' un '\n'
se lo e' ritorna vero altrimenti falso
*/
bool	is_newline_char(char buffer_char)
{
	return (buffer_char == '\n');
}

/*
aggiunge un carattere alla linea corrente
->controlla che len della linea non sia maggiore
di max_line_length, se cosi' aggiunge il char
alla linea e incrementa il contatore line_len
*/
void	add_char_to_line(char buffer_char, char *line, size_t *line_len)
{
	if (*line_len < MAX_LINE_LENGTH - 1)
	{
		line[(*line_len)++] = buffer_char;
	}
}

bool	check_exit_condition(size_t line_len)
{
	return (line_len == 0);
}

/*
se i byte letti sono 0 [->eof] e la len della linea e' 0
ritorna true al chiamante, read_input_line, ed esce dal loop
*/
bool	update_exit_status(ssize_t bytes_read, size_t line_len)
{
	if (bytes_read == 0 && check_exit_condition(line_len))
		return (true);
	return (false);
}
