/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 10:07:22 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/27 13:26:45 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
perror e free se dup2 fallisce su fd[0]
esce dal figlio con exit_failure
*/
void	call_perror_child_process_fd1(t_command *current_cmd)
{
	perror("dup2");
	free_command_list(current_cmd);
	exit(EXIT_FAILURE);
}

/*
perror e free se dup2 fallisce su fd[1]
esce dal figlio con exit_failure
*/
void	call_perror_child_process_fd2(t_command *current_cmd)
{
	perror("dup2");
	free_command_list(current_cmd);
	exit(EXIT_FAILURE);
}

/*
gestisce pulizia fds nella pipeline
se la precedente e' stata usata(suff controllo su lato lettura)
chiude lato lettura e lato scrittura
aggiorna i campi prev_pipe_fd lettura e scrittura
con i campi correnti->per la prossima iterazione
*/
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

/*
chiamata se fork fallisce
*/
void	call_perror_exec_pipeline(t_command *command_list)
{
	perror("fork");
	free_command_list(command_list);
	exit(EXIT_FAILURE);
}

/*
chiude entrambi gli fds della pipe precedente
*/
void	close_prev_pipe_fd(int prev_pipe_fd[2])
{
	close(prev_pipe_fd[0]);
	close(prev_pipe_fd[1]);
}
