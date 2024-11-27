/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_wait_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 10:06:14 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/27 13:51:27 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
crea pipe se c'e un comando successivo[un altra pipe]
-se c'e chiama pipe per creare gli fd, + gestisce errore
-se non c'e inizializza gli fd a -1 per indicare
che non sono stati creati
*/
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

/*
attende fine di tutti i figli
-usa wait per conoscere stato di uscita di ogni figlio
se wait>0 -> ci sono ancora figli da attedere
-aggiorna last_exit_status con codice uscita ultimo figlio
-wifexited -> uscito normalmente con exit
-wifsignaled -> uscito con segnale
*/
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

/*
inizializza fd pipe precedente e il puntatore
al cmd corrente
-e' il primo della pipeline quindi non ci sono prev fds
*/
void	init_pipeline(t_command **current_cmd,
			int prev_pipe_fd[2], t_command *command_list)
{
	prev_pipe_fd[0] = -1;
	prev_pipe_fd[1] = -1;
	*current_cmd = command_list;
}
