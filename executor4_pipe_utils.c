/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor4_pipe_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:45:45 by tschetti          #+#    #+#             */
/*   Updated: 2024/10/07 20:55:25 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

void	call_perror_child_process_fd1(t_command *current_cmd)
{
	perror("dup2");
	free_command_list(current_cmd);
	exit(EXIT_FAILURE);
}

void	call_perror_child_process_fd2(t_command *current_cmd)
{
	perror("dup2");
	free_command_list(current_cmd);
	exit(EXIT_FAILURE);
}

void	parent_cleanup(int prev_pipe_fd[2], int pipe_fd[2])
{
	if (prev_pipe_fd[0] != -1)
	{
		close(prev_pipe_fd[0]);
		close(prev_pipe_fd[1]);
	}
	prev_pipe_fd[0] = pipe_fd[0];
	prev_pipe_fd[1] = pipe_fd[1];
}

void	call_perror_exec_pipeline(t_command *command_list)
{
	perror("fork");
	free_command_list(command_list);
	exit(EXIT_FAILURE);
}

void	close_prev_pipe_fd(int prev_pipe_fd[2])
{
	close(prev_pipe_fd[0]);
	close(prev_pipe_fd[1]);
}
