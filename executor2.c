/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:45:41 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/01 12:36:13 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

void	init_io_fds(t_io_fds *fds)
{
	fds->infile_fd = -1;
	fds->outfile_fd = -1;
	fds->heredoc_fd = -1;
	fds->stdin_backup = -1;
	fds->stdout_backup = -1;
}

void	restore_standard_fds(t_io_fds *fds)
{
	if (fds->stdin_backup != -1)
	{
		if (dup2(fds->stdin_backup, STDIN_FILENO) < 0)
		{
			perror("Error restoring stdin");
			close(fds->stdin_backup);
			fds->stdin_backup = -1;
			return ;
		}
		close(fds->stdin_backup);
		fds->stdin_backup = -1;
	}
	if (fds->stdout_backup != -1)
	{
		if (dup2(fds->stdout_backup, STDOUT_FILENO) < 0)
		{
			perror("Error restoring stdout");
			close(fds->stdout_backup);
			fds->stdout_backup = -1;
			return ;
		}
		close(fds->stdout_backup);
		fds->stdout_backup = -1;
	}
}

void	execute_builtin_command(t_command *command, char **args_array,
				bool *args_quote_flags, t_shell_state *shell_state)
{
	execute_builtin(command, args_array, args_quote_flags, shell_state);
	shell_state->last_exit_status = 0;
}

void execute_external_command(t_command* all_cmds, t_command* command, char** args_array, t_shell_state* shell_state)
{
	char	*full_path;
	char	**envp;

	envp = build_envp(shell_state);
	if (!envp)
	{
		perror("build_envp");
		free_command_list(all_cmds);
		free_args_array(args_array);
		free_args_array(envp);
		exit(EXIT_FAILURE);
	}
	full_path = get_command_path(command->cmd_name, shell_state);
	clean_shell_state(shell_state);
	shell_state->env_list = NULL;
	if (!full_path)
	{
		printf("Command not found: %s\n", command->cmd_name);
		free_command_list(all_cmds);
		free_args_array(args_array);
		free_args_array(envp);
		clean_shell_state(shell_state);
		exit(127);
	}
	execve(full_path, args_array, envp);
	perror("execve");
	free(full_path);
	free_command_list(all_cmds);
	free_args_array(args_array);
	free_args_array(envp);
	exit(EXIT_FAILURE);
}
