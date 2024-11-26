/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_read_line.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 19:08:30 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/26 19:35:18 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

#define MAX_LINE_LENGTH 4096

/*
gestisce errori durante la lettura
se read fallisce chiama perror e termina
se read fallisce ma l errore e' 
eintr (interruzione tramite segnale) termina
senza chiamare perror
*/
bool	handle_read_error(ssize_t bytes_read)
{
	if (bytes_read == -1 && errno == EINTR)
	{
		return (true);
	}
	else if (bytes_read == -1)
	{
		perror("read");
		return (true);
	}
	return (false);
}

/*
legge una riga di input, carattere per carattere
-len della linea inizializzata a 0
-flag per uscire dal loop
esce se:
c'e un errore di lettura-> handle_read_error
raggiunge fine file
c'e un carattere new_line
altrimenti aggiunge il carattere letto alla linea e aggiorna len
*/
bool	read_input_line(char *line, size_t *line_len)
{
	char	buffer[1];
	ssize_t	bytes_read;
	bool	should_exit;

	*line_len = 0;
	should_exit = false;
	while (1)
	{
		bytes_read = read_single_char(buffer);
		if (handle_read_error(bytes_read))
		{
			should_exit = true;
			break ;
		}
		should_exit = update_exit_status(bytes_read, *line_len);
		if (should_exit)
			break ;
		if (is_newline_char(buffer[0]))
			break ;
		else
			add_char_to_line(buffer[0], line, line_len);
	}
	line[*line_len] = '\0';
	return (should_exit);
}
