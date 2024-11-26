/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_signals.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:05:20 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/26 18:05:22 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
imposta i tre segnali al loro
comportamento di default
*/
void	handle_signals_in_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
}

/*
ripristina il gestore dei segnali per sigint e sigquit
chiamano handle_sigint e handle_sigquit rispettivamente
resettano la maschera dei segnali, 0->nessuna opzione,
sigaction applica effetivamente il gestore
*/
void	restore_signals_after_command(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = handle_sigquit;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

/*
ignora sigint e sigquit nel padre
*/
void	handle_signals_in_parent(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

/*
gestisce sigint durante heredoc
la var globale assume valore del segnale
*/
void	handle_sigint_heredoc(int sig)
{
	g_received_signal = sig;
}

/*
gestisce la fork
nel figlio, pid=0
-imposta segnali al comportamento di default
-esegue il comando
nel padre, pid>0
-ignora i segnali
-attende fine del figlio
-ripristina originale gestione dei segnali
-se fork fallisce(pid<0), perror e exit_staut=1
*/
void	handle_fork(t_command *all_cmds, t_fork_info *finfo,
			t_shell_state *shell_state)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		handle_signals_in_child();
		exec_in_child(all_cmds, finfo, shell_state);
	}
	else if (pid > 0)
	{
		handle_signals_in_parent();
		handle_parent_after_fork(pid, shell_state);
		restore_signals_after_command();
	}
	else
	{
		perror("fork");
		shell_state->last_exit_status = 1;
	}
}
