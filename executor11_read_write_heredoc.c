/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor11_read_write_heredoc.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 01:20:53 by tschetti          #+#    #+#             */
/*   Updated: 2024/10/17 03:29:00 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

#define MAX_LINE_LENGTH 4096

void	expand_and_write_line(const char *line, int pipefd[2],
				bool is_quoted, t_shell_state *shell_state)
{
	char	*expanded_line;

	if (!is_quoted)
	{
		expanded_line = expand_var_in_heredoc(line, shell_state);
		if (expanded_line)
			write(pipefd[1], expanded_line, strlen(expanded_line));
		else
			write(pipefd[1], line, strlen(line));
	}
	else
		write(pipefd[1], line, strlen(line));
	write(pipefd[1], "\n", 1);
}

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

void	read_and_expand_heredoc(const char *delimiter, int pipefd[2],
				bool is_quoted, t_shell_state *shell_state)
{
	char	line[MAX_LINE_LENGTH];
	size_t	line_len;
	bool	should_exit;

	should_exit = false;
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		should_exit = read_input_line(line, &line_len);
		if (should_exit)
		{
			write(STDOUT_FILENO, "\n", 1);
			break ;
		}
		if (strcmp(line, delimiter) == 0)
			break ;
		expand_and_write_line(line, pipefd, is_quoted, shell_state);
		if (g_received_signal != 0)
			break ;
	}
}
