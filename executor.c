/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:45:40 by tschetti          #+#    #+#             */
/*   Updated: 2024/10/29 12:19:18 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

#define MAX_LINE_LENGTH 4096

int	prepare_arguments(t_command *command, char ***args_array,
				bool **args_quote_flags)
{
	*args_array = convert_arglist_for_exc(command, args_quote_flags);
	if (!*args_array)
	{
		perror("convert_arglist_for_exc");
		return (-1);
	}
	return (0);
}

void	exec_bltn_in_parent(t_command *command, char **args_array,
			bool *args_quote_flags, t_shell_state *shell_state)
{
	t_io_fds	fds;

	init_io_fds(&fds);
	if (handle_builtin_redirections(command, &fds, shell_state) != 0)
	{
		restore_standard_fds(&fds);
		return ;
	}
	execute_builtin(command, args_array, args_quote_flags, shell_state);
	restore_standard_fds(&fds);
}

void	exec_in_child(t_command *command, char **args_array,
			bool *args_quote_flags, t_shell_state *shell_state)
{
	int			is_builtin_cmd;
	t_io_fds	fds;

	init_io_fds(&fds);
	is_builtin_cmd = is_builtin(command->cmd_name);
	handle_child_redirections(command, &fds, shell_state);
	if (is_builtin_cmd)
	{
		execute_builtin(command, args_array, args_quote_flags, shell_state);
		exit(shell_state->last_exit_status);
	}
	else
	{
		execute_external_command(command, args_array, shell_state);
	}
}

void	handle_parent_after_fork(pid_t pid, t_shell_state *shell_state)
{
	int	status;
	int	sig;

	if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			shell_state->last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			shell_state->last_exit_status = 128 + sig;
			if (sig == SIGQUIT)
				write(STDERR_FILENO, "Quit (core dumped)\n", 19);
			else if (sig == SIGINT)
				write(STDOUT_FILENO, "\n", 1);
		}
	}
	else
	{
		perror("fork");
		shell_state->last_exit_status = 1;
	}
}

void	expand_and_write_line(const char *line, int fd,
				bool is_quoted, t_shell_state *shell_state)
{
	char	*expanded_line;

	if (!is_quoted)
	{
		expanded_line = expand_var_in_heredoc(line, shell_state);
		if (expanded_line)
			write(fd, expanded_line, strlen(expanded_line));
		else
			write(fd, line, strlen(line));
	}
	else
		write(fd, line, strlen(line));
	write(fd, "\n", 1);
}

void	handle_sigint_heredoc(int sig)
{
	g_received_signal = sig;
}

void	read_and_expand_heredoc(const char *delimiter, int fd, bool is_quoted, t_shell_state *shell_state)
{
	char				line[MAX_LINE_LENGTH];
	size_t				line_len;
	bool				should_exit;
	struct sigaction	sa_int;
	struct sigaction	sa_quit;
	struct sigaction	sa_old_int;
	struct sigaction	sa_old_quit;

	sa_int.sa_handler = handle_sigint_heredoc;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, &sa_old_int);
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, &sa_old_quit);
	should_exit = false;
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		should_exit = read_input_line(line, &line_len);
		if (should_exit || g_received_signal == SIGINT)
		{
			write(STDOUT_FILENO, "\n", 1);
			break ;
		}
		if (strcmp(line, delimiter) == 0)
			break ;
		expand_and_write_line(line, fd, is_quoted, shell_state);
	}
	sigaction(SIGINT, &sa_old_int, NULL);
	sigaction(SIGQUIT, &sa_old_quit, NULL);
}

void	handle_heredoc(t_redirection *redirection, char **heredoc_filename, t_shell_state *shell_state)
{
	char	tmp_filename[] = "/tmp/heredocXXXXXX";
	int	fd = mkstemp(tmp_filename);

	if (fd == -1)
	{
		perror("mkstemp");
		*heredoc_filename = NULL;
		return ;
	}
	*heredoc_filename = ft_strdup(tmp_filename);
	if (!*heredoc_filename)
	{
		perror("ft_strdup");
		close(fd);
		unlink(tmp_filename);
		*heredoc_filename = NULL;
		return ;
	}
	printf("handle_heredoc: heredoc_filename = %s\n", *heredoc_filename);
	read_and_expand_heredoc(redirection->filename, fd, redirection->is_quoted, shell_state);
	close(fd);
}

int	process_all_heredocs(t_command *command_list, t_shell_state *shell_state)
{
	t_command	*current_cmd = command_list;
	while (current_cmd)
	{
		t_redirection	*redirection = current_cmd->redirections;
		while (redirection)
		{
			if (redirection->type == TOKEN_HEREDOC)
			{
				char	*heredoc_filename = NULL;
				handle_heredoc(redirection, &heredoc_filename, shell_state);
				if (!heredoc_filename)
				{
					fprintf(stderr, "Failed to create heredoc file for redirection\n");
					return (-1);
				}
				redirection->heredoc_filename = heredoc_filename;
			}
			redirection = redirection->next;
		}
		current_cmd = current_cmd->next;
	}
	return (0);
}

void	execute_single_command(t_command *command, t_shell_state *shell_state)
{
	char	**args_array;
	bool	*args_quote_flags;
	int		is_builtin_cmd;

	if (initialize_command_args(command, &args_array, &args_quote_flags) != 0)
		return ;
	is_builtin_cmd = is_builtin(command->cmd_name);
	if (is_builtin_cmd && command->next == NULL)
		exec_bltn_in_parent(command, args_array, args_quote_flags, shell_state);
	else
		handle_fork(command, args_array, args_quote_flags, shell_state);
	t_redirection	*redirection = command->redirections;
	while (redirection)
	{
		if (redirection->type == TOKEN_HEREDOC)
		{
			unlink(redirection->heredoc_filename);
			free(redirection->heredoc_filename);
			redirection->heredoc_filename = NULL;
		}
	redirection = redirection->next;
	}
	free_args_array(args_array);
	free(args_quote_flags);
}
