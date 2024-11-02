/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: negambar <negambar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:45:40 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/02 15:30:58 by negambar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

#define MAX_LINE_LENGTH 4096
#define MAX_ATTEMPTS 100

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

void exec_bltn_in_parent(t_command* command, char** args_array, bool* args_quote_flags, t_shell_state* shell_state)
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

void exec_in_child(t_command* all_cmds, t_fork_info* finfo, t_shell_state* shell_state)
{
	int			is_builtin_cmd;
	t_io_fds	fds;

	init_io_fds(&fds);
	is_builtin_cmd = is_builtin(finfo->cmd->cmd_name);
	handle_child_redirections(finfo->cmd, &fds, shell_state);
	if (is_builtin_cmd)
	{
		execute_builtin(finfo->cmd, finfo->args_array, finfo->args_quote_flags, shell_state);
		free_args_array(finfo->args_array);
		free(finfo->args_quote_flags);
		free_command_list(all_cmds);
		clean_shell_state(shell_state);
		exit(shell_state->last_exit_status);
	}
	else
	{
		free(finfo->args_quote_flags);
		execute_external_command(all_cmds, finfo->cmd, finfo->args_array, shell_state);
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
			/* if (sig == SIGQUIT)
				write(STDERR_FILENO, "Quit (core dumped)\n", 19);
			else  */if (sig == SIGINT)
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

void	heredoc_input_loop(const char	*delimiter, int fd,
			bool is_quoted, t_shell_state *shell_state)
{
	char	line[MAX_LINE_LENGTH];
	size_t	line_len;
	bool	should_exit;

	should_exit = false;
	while (1)
	{
		write(1, "> ", 2);
		should_exit = read_input_line(line, &line_len);
		if (should_exit || g_received_signal == SIGINT)
		{
			write(1, "\n", 1);
			break ;
		}
		if (strcmp(line, delimiter) == 0)
			break ;
		expand_and_write_line(line, fd, is_quoted, shell_state);
	}
}

void	read_and_expand_heredoc(const char *delimiter, int fd,
				bool is_quoted, t_shell_state *shell_state)
{
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
	heredoc_input_loop(delimiter, fd, is_quoted, shell_state);
	sigaction(SIGINT, &sa_old_int, NULL);
	sigaction(SIGQUIT, &sa_old_quit, NULL);
}

void	init_int_to_str_vars(t_int_to_str_vars *vars, int num)
{
	vars->i = 0;
	vars->is_negative = 0;
	vars->temp_num = num;
	if (num == 0)
	{
		vars->temp_str[vars->i++] = '0';
		vars->temp_str[vars->i] = '\0';
	}
	else if (num < 0)
	{
		vars->is_negative = 1;
		vars->temp_num = -num;
	}
}

void	int_to_str(int num, char *str)
{
	t_int_to_str_vars	vars;

	init_int_to_str_vars(&vars, num);
	if (num == 0)
	{
		str[0] = vars.temp_str[0];
		str[1] = '\0';
		return ;
	}
	while (vars.temp_num > 0)
	{
		vars.temp_str[vars.i++] = (vars.temp_num % 10) + '0';
		vars.temp_num /= 10;
	}
	if (vars.is_negative)
		vars.temp_str[vars.i++] = '-';
	vars.j = 0;
	while (vars.i > 0)
		str[vars.j++] = vars.temp_str[--vars.i];
	str[vars.j] = '\0';
}

int	generate_unique_filename(t_filename_vars *vars)
{
	vars->unique_id = 0;
	while (vars->unique_id < MAX_ATTEMPTS)
	{
		ft_strcpy(vars->tmp_filename, "/tmp/heredoc_");
		int_to_str(vars->unique_id, vars->unique_id_str);
		ft_strcat(vars->tmp_filename, vars->unique_id_str);
		vars->fd = open(vars->tmp_filename, O_CREAT | O_EXCL | O_RDWR, 0600);
		if (vars->fd != -1)
			return (0);
		if (errno != EEXIST)
		{
			perror("open");
			return (-1);
		}
		vars->unique_id++;
	}
	write(2, "Failed to create unique heredoc file\n", 37);
	return (-1);
}

int	create_temp_heredoc_file(char **heredoc_filename)
{
	t_filename_vars	vars;

	if (generate_unique_filename(&vars) != 0)
	{
		*heredoc_filename = NULL;
		return (-1);
	}
	*heredoc_filename = ft_strdup(vars.tmp_filename);
	if (!*heredoc_filename)
	{
		perror("ft_strdup");
		close(vars.fd);
		unlink(vars.tmp_filename);
		*heredoc_filename = NULL;
		return (-1);
	}
	return (vars.fd);
}

void	handle_heredoc(t_redirection *redirection,
			char **heredoc_filename, t_shell_state *shell_state)
{
	int	fd;

	fd = create_temp_heredoc_file(heredoc_filename);
	if (fd == -1)
		return ;
	read_and_expand_heredoc(redirection->filename, fd,
		redirection->is_quoted, shell_state);
	close(fd);
}

int	process_command_heredocs(t_command *current_cmd, t_shell_state *shell_state)
{
	t_redirection	*redirection;
	char			*heredoc_filename;

	redirection = current_cmd->redirections;
	while (redirection)
	{
		if (redirection->type == TOKEN_HEREDOC)
		{
			heredoc_filename = NULL;
			handle_heredoc(redirection, &heredoc_filename, shell_state);
			if (!heredoc_filename)
			{
				write(2, "failed to create heredoc file 4 redir\n", 38);
				return (-1);
			}
			redirection->heredoc_filename = heredoc_filename;
		}
		redirection = redirection->next;
	}
	return (0);
}

int	process_all_heredocs(t_command *command_list, t_shell_state *shell_state)
{
	t_command		*current_cmd;

	current_cmd = command_list;
	while (current_cmd)
	{
		if (process_command_heredocs(current_cmd, shell_state) < 0)
			return (1);
		current_cmd = current_cmd->next;
	}
	return (0);
}

void execute_single_command(t_command* all_cmds, t_command* command, t_shell_state* shell_state)
{
	char			**args_array;
	bool			*args_quote_flags;
	int				is_builtin_cmd;
	t_redirection	*redirection;
	t_fork_info		fork_info;

	redirection = command->redirections;
	if (initialize_command_args(command, &args_array, &args_quote_flags) != 0)
		return ;
	is_builtin_cmd = is_builtin(command->cmd_name);
	fork_info = (t_fork_info){command, args_array, args_quote_flags};
	if (is_builtin_cmd && command->next == NULL)
		exec_bltn_in_parent(command, args_array, args_quote_flags, shell_state);
	else
		handle_fork(all_cmds, &fork_info, shell_state);
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
