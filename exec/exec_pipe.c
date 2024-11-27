/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 10:07:13 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/27 12:49:09 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
chiude file descriptor della pipe precedente e attende i figli
-se fd della pipe precedente e' aperto (valore diverso da -1)
->chiude fd della pipe precedente (basta prev_fd[0]) e attende figli, inoltre
ripristina gestore dei segnali custom
*/
void	finalize_pipeline(int prev_pipe_fd[2], t_shell_state *shell_state)
{
	if (prev_pipe_fd[0] != -1)
		close_prev_pipe_fd(prev_pipe_fd);
	wait_for_all_children(shell_state);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
}

/*
gestisce figlio nella pipeline
-ripristina comportamento di default per sigint e sigquit
-chiama setup_child_process per gestire
effettivamente figlio nella pipeline e eseguire il cmd
*/
void	handle_child_process(t_command *all_cmds, t_pipe_cmd *pcmd,
			t_shell_state *shell_state)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	setup_child_prcs(all_cmds, pcmd, shell_state);
}

/*
gestisce padre durante la pipeline
-ignora sigint e sigquit
-pulisce fd della pipeline->parent_cleanup
chiude gli fd della pipe precedente se necessario
reimposta i prev_pipe_fd per la prossima iterazione
*/
void	handle_parent_process(pid_t pid, int prev_pipe_fd[2], int pipe_fd[2])
{
	(void)pid;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	parent_cleanup(prev_pipe_fd, pipe_fd);
}

/*
ciclo per pipeline
crea le pipe e e forka ogni comando via via
gestisce figlio(esecuzione comando) e padre(chiusura
pipe e attesa figli) per ogni cmd
*/
void	execute_pipeline_loop(t_command *current_cmd, int prev_pipe_fd[2],
				t_command *command_list, t_shell_state *shell_state)
{
	int			pipe_fd[2];
	pid_t		pid;
	t_pipe_cmd	pcmd;

	while (current_cmd)
	{
		pipe_fd[0] = -1;
		pipe_fd[1] = -1;
		create_pipe_if_needed(current_cmd, pipe_fd);
		pcmd = (t_pipe_cmd){current_cmd, prev_pipe_fd, pipe_fd};
		pid = fork();
		if (pid == 0)
			handle_child_process(command_list, &pcmd, shell_state);
		else if (pid > 0)
			handle_parent_process(pid, prev_pipe_fd, pipe_fd);
		else
			call_perror_exec_pipeline(command_list);
		current_cmd = current_cmd->next;
	}
}

/*
esegue pipepline, chiamante execute_commands
-inizializza stato della pipeline
-esegue ogni comando nella pipeline via via
-finalizza->chiude pipe, attende figli
*/
void	execute_pipeline(t_command *command_list, t_shell_state *shell_state)
{
	t_command	*current_cmd;
	int			prev_pipe_fd[2];

	init_pipeline(&current_cmd, prev_pipe_fd, command_list);
	execute_pipeline_loop(current_cmd, prev_pipe_fd, command_list, shell_state);
	finalize_pipeline(prev_pipe_fd, shell_state);
}
