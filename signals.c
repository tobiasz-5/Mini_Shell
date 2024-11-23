/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 20:25:41 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/20 20:25:41 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

/*
variabile volatile -> tipo speciale di variabile
assicura che il programma legga sempre il valore aggiornato 
(-> per variabili modificate da segnali o eventi esterni -> anche hardware)
compilatore -> ottimizza accesso alle variabili conservandone 
una copia in registri della CPU o in cache, 
evitando letture ripetute dalla memoria -> però può causare problemi se una
variabile viene modificata da eventi esterni (come segnali o hardware).
se e' volatile il compilatore non la otttimizza
*/
volatile sig_atomic_t	g_received_signal = 0;

void	handle_sigint(int sig)
{
	(void)sig;
	g_received_signal = sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigquit(int sig)
{
	(void)sig;
}

// void	handle_sigpipe(int sig)
// {
// 	(void)sig;
// }

/*
struct sigaction -> struttura di sistema
per configurare la gestione dei segnali in sist UNIX/linux
  ------------------------------------------------------
__.sa_handler -> la funzione da eseguire quando viene ricevuto un certo segnale.
sigemptyset -> non bloccare altri segnali mentra handle_sigint sta lavorando
sa_int.sa_flags = 0;--> no flags per sa_handler -> default behaviour
sigaction->
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
signum -> Specifica il segnale da configurare
*act -> puntatore alla stuttura sigaction -> contiene la nuova config del segnale
*oldact -> NULL nel nostro codice, ma puo servire a 
salvare la vecchia config (sa_handler, sa_mask, sa_flags) del segnale
*/
void	init_sign(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;
	struct sigaction	sa_pipe;

	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = handle_sigquit;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
	sa_pipe.sa_handler = SIG_IGN;
	sigemptyset(&sa_pipe.sa_mask);
	sa_pipe.sa_flags = 0;
	sigaction(SIGPIPE, &sa_pipe, NULL);
}

void	update_last_exit_status(t_shell_state *shell_state)
{
	if (g_received_signal)
	{
		shell_state->last_exit_status = 128 + g_received_signal;
		g_received_signal = 0;
	}
}
