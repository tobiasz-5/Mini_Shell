/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor3_setup_child.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:36:14 by tschetti          #+#    #+#             */
/*   Updated: 2024/10/16 19:39:43 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

void	init_fds_bye_norminette(t_io_fds *fds)
{
	fds->infile_fd = -1;
	fds->outfile_fd = -1;
	fds->heredoc_fd = -1;
	fds->stdin_backup = -1;
	fds->stdout_backup = -1;
}

void	close_pipe_fds(int prev_pipe_fd[2], int pipe_fd[2])
{
	if (prev_pipe_fd[0] != -1)
	{
		close(prev_pipe_fd[0]);
		close(prev_pipe_fd[1]);
	}
	if (pipe_fd[0] != -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
}

void	setup_child_prcs(t_command *current_cmd, int prev_pipe_fd[2],
				int pipe_fd[2], t_shell_state *shell_state)
{
	t_io_fds	fds;

	init_fds_bye_norminette(&fds);
	if (prev_pipe_fd[0] != -1)
	{
		if (dup2(prev_pipe_fd[0], STDIN_FILENO) == -1)
			call_perror_child_process_fd1(current_cmd);
	}
	if (pipe_fd[1] != -1)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			call_perror_child_process_fd2(current_cmd);
	}
	close_pipe_fds(prev_pipe_fd, pipe_fd);
	handle_child_redirections(current_cmd, &fds, shell_state);
	execute_single_command(current_cmd, shell_state);
	exit(shell_state->last_exit_status);
}
