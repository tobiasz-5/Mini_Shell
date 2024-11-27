/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 10:06:59 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/27 13:17:40 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
inizializza con valori di default
-1 indica non utilizzati
*/
void	init_fds_bye_norminette(t_io_fds *fds)
{
	fds->infile_fd = -1;
	fds->outfile_fd = -1;
	fds->heredoc_fd = -1;
	fds->stdin_backup = -1;
	fds->stdout_backup = -1;
}

/*
chiude fds delle pipe precedenti e correnti,
se sono stati utilizzati [-1]
*/
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

/*
configura processo figlio per pipeline
-se l fd[0] e' stata aperto/utilizzato [val diverso da -1]
allora duplica file descriptor della pipe precedente
(lettura) su stdin-> cosi comando puo leggere
dall output del cmd precedente
-se l fd[1] e' stato aperto/utilizzato 
duplica file descriptor della pipe corrente (scrittura)
su stdout-> cosi comando puo inviare/scrivere il suo output
nell fd per ill cmd successivo
-poi chiude gli fds se necessario
-esegue il comando
-libera risorse ed esce dal figlio
*/
void	setup_child_prcs(t_command *all_cmds, t_pipe_cmd *pcmd,
			t_shell_state *shell_state)
{
	t_io_fds	fds;

	init_fds_bye_norminette(&fds);
	if (pcmd->prev_pipe_fd[0] != -1)
	{
		if (dup2(pcmd->prev_pipe_fd[0], STDIN_FILENO) == -1)
			call_perror_child_process_fd1(pcmd->cmd);
	}
	if (pcmd->pipe_fd[1] != -1)
	{
		if (dup2(pcmd->pipe_fd[1], STDOUT_FILENO) == -1)
			call_perror_child_process_fd2(pcmd->cmd);
	}
	close_pipe_fds(pcmd->prev_pipe_fd, pcmd->pipe_fd);
	handle_child_redirections(pcmd->cmd, &fds, shell_state);
	execute_single_command(all_cmds, pcmd->cmd, shell_state);
	free_command_list(all_cmds);
	clean_shell_state(shell_state);
	exit(shell_state->last_exit_status);
}
