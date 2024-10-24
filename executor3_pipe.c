/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor3_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:45:43 by tschetti          #+#    #+#             */
/*   Updated: 2024/10/22 13:39:38 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

void	finalize_pipeline(int prev_pipe_fd[2], t_shell_state *shell_state)
{
	if (prev_pipe_fd[0] != -1)
		close_prev_pipe_fd(prev_pipe_fd);
	wait_for_all_children(shell_state);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
}

void	handle_child_process(t_command *current_cmd, int prev_pipe_fd[2],
					int pipe_fd[2], t_shell_state *shell_state)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	setup_child_prcs(current_cmd, prev_pipe_fd, pipe_fd, shell_state);
}

void	handle_parent_process(pid_t pid, int prev_pipe_fd[2], int pipe_fd[2])
{
	(void)pid;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	parent_cleanup(prev_pipe_fd, pipe_fd);
}

void	execute_pipeline_loop(t_command *current_cmd, int prev_pipe_fd[2],
				t_command *command_list, t_shell_state *shell_state)
{
	int		pipe_fd[2];
	pid_t	pid;

	while (current_cmd)
	{
		pipe_fd[0] = -1;
		pipe_fd[1] = -1;
		create_pipe_if_needed(current_cmd, pipe_fd);
		pid = fork();
		if (pid == 0)
			handle_child_process(current_cmd, prev_pipe_fd,
				pipe_fd, shell_state);
		else if (pid > 0)
			handle_parent_process(pid, prev_pipe_fd, pipe_fd);
		else
			call_perror_exec_pipeline(command_list);
		current_cmd = current_cmd->next;
	}
}

void	execute_pipeline(t_command *command_list, t_shell_state *shell_state)
{
	t_command	*current_cmd;
	int			prev_pipe_fd[2];

	init_pipeline(&current_cmd, prev_pipe_fd, command_list);
	execute_pipeline_loop(current_cmd, prev_pipe_fd, command_list, shell_state);
	finalize_pipeline(prev_pipe_fd, shell_state);
}
