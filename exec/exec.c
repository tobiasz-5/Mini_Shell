/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girindi <girindi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:45:40 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/05 17:05:54 by girindi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

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

void	exec_in_child(t_command *all_cmds, t_fork_info *finfo,
			t_shell_state *shell_state)
{
	int			is_builtin_cmd;
	t_io_fds	fds;

	init_io_fds(&fds);
	is_builtin_cmd = is_builtin(finfo->cmd->cmd_name);
	handle_child_redirections(finfo->cmd, &fds, shell_state);
	if (is_builtin_cmd)
	{
		execute_builtin(finfo->cmd, finfo->args_array,
			finfo->args_quote_flags, shell_state);
		free_args_array(finfo->args_array);
		free(finfo->args_quote_flags);
		free_command_list(all_cmds);
		clean_shell_state(shell_state);
		exit(shell_state->last_exit_status);
	}
	else
	{
		free(finfo->args_quote_flags);
		exec_ext_cmd(all_cmds, finfo->cmd,
			finfo->args_array, shell_state);
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

void	execute_single_command(t_command *all_cmds,
			t_command *command, t_shell_state *shell_state)
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
			clean_filename(redirection);
		redirection = redirection->next;
	}
	free_args_array(args_array);
	free(args_quote_flags);
}
