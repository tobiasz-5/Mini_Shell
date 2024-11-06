/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor3_wait.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: negambar <negambar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 20:55:30 by girindi          #+#    #+#             */
/*   Updated: 2024/11/05 16:10:38 by negambar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

void	create_pipe_if_needed(t_command *current_cmd, int pipe_fd[2])
{
	if (current_cmd->next)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		pipe_fd[0] = -1;
		pipe_fd[1] = -1;
	}
}

void	wait_for_all_children(t_shell_state *shell_state)
{
	int	status;
	int	sig;

	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			shell_state->last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			shell_state->last_exit_status = 128 + sig;
			if (sig == SIGQUIT)
				write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
			else if (sig == SIGINT)
				write(STDOUT_FILENO, "\n", 1);
		}
	}
}

void	init_pipeline(t_command **current_cmd,
			int prev_pipe_fd[2], t_command *command_list)
{
	prev_pipe_fd[0] = -1;
	prev_pipe_fd[1] = -1;
	*current_cmd = command_list;
}
