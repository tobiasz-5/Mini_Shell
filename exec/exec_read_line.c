/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor11_read_write_heredoc.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: negambar <negambar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 01:20:53 by girindi          #+#    #+#             */
/*   Updated: 2024/11/05 16:10:38 by negambar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

#define MAX_LINE_LENGTH 4096

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
