/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor12_read_and_expand_utils.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 01:46:45 by tschetti          #+#    #+#             */
/*   Updated: 2024/10/16 01:46:46 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

#define MAX_LINE_LENGTH 4096

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

bool	is_newline_char(char buffer_char)
{
	return (buffer_char == '\n');
}

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

bool	update_exit_status(ssize_t bytes_read, size_t line_len)
{
	if (bytes_read == 0 && check_exit_condition(line_len))
		return (true);
	return (false);
}
