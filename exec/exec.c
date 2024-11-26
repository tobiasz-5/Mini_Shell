/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 14:19:19 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/26 15:31:20 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
esegue built-in non in pipeline
-> salva stdin originale per ripristinarlo dopo
-> inizializza fds per le redirezioni
gestisce le redir del comando se ci sono
->se fallisce ripristina gli fds e termina
altrimenti esegue il built-in, ripristina gli fds,
ripristina lo stdin originale e 
chiude il file descriptor duplicato
*/
void	exec_bltn_in_parent(t_command *command, char **args_array,
			bool *args_quote_flags, t_shell_state *shell_state)
{
	t_io_fds	fds;
	int			old_stdin_fd;

	old_stdin_fd = dup(STDIN_FILENO);
	init_io_fds(&fds);
	if (handle_builtin_redirections(command, &fds, shell_state) != 0)
	{
		restore_standard_fds(&fds);
		return ;
	}
	execute_builtin(command, args_array, args_quote_flags, shell_state);
	restore_standard_fds(&fds);
	dup2(old_stdin_fd, STDIN_FILENO);
	close(old_stdin_fd);
}

/*
esegue comando in un processo figlio
-> inizializza gli fds per le redir
-> determina se il comando e' un bltn
-> gestisce le redir per il figlio, se fallisce, libera
-> se e' un bltn, esegue e libera
altrimenti libera flag quotes ed esegue il
comando non built-in
*/
void	exec_in_child(t_command *all_cmds, t_fork_info *finfo,
			t_shell_state *shell_state)
{
	int			is_builtin_cmd;
	t_io_fds	fds;

	init_io_fds(&fds);
	is_builtin_cmd = is_builtin(finfo->cmd->cmd_name);
	if (!handle_child_redirections(finfo->cmd, &fds, shell_state))
		free_all(all_cmds, finfo, shell_state);
	if (is_builtin_cmd)
	{
		execute_builtin(finfo->cmd, finfo->args_array,
			finfo->args_quote_flags, shell_state);
		free_all(all_cmds, finfo, shell_state);
	}
	else
	{
		free(finfo->args_quote_flags);
		exec_ext_cmd(all_cmds, finfo->cmd,
			finfo->args_array, shell_state);
	}
}

/*
gestisce comportamento processo padre dopo fork o 
se fork fallisce
->attende terminazione processo figlio waitpid
se figlio termina normalmente->salva codice uscita del figlio
in last_exit_statu
se figlio termina per un segnale ricevuto->aggiorna last_exit_status
128+sig-> poi in base al tipo di segno applica due write differenti
se invece fork fallisce mostra errore e 
aggiorna last_exit_status
*/
void	handle_parent_after_fork(pid_t pid, t_shell_state *shell_state)
{
	int	status;
	int	sig;

	if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			shell_state->last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			shell_state->last_exit_status = 128 + sig;
			if (sig == SIGQUIT)
				write(STDERR_FILENO, "Quit (core dumped)\n", 19);
			else if (sig == SIGINT)
				write(STDOUT_FILENO, "\n", 1);
		}
	}
	else
	{
		perror("fork");
		shell_state->last_exit_status = 1;
	}
}

/*
esegue un singolo comando
-> inizializza puntatore redirection per poter
iterare sulla lista di redir
-> inizializza comando, suoi argomenti, sue flag quotes.
->se il comando e' un builtin e non e' in pipeline,
command->next == null, lo esegue senza creare processi figlio->
exec_bltn_in_parent, altrimenti, se c'e' una pipeline o e' 
un cmd singolo ma non un blt-in deve gestire la fork->
handle_fork
cicla sulla lista di redirezioni per liberare eventuali
risorse associate ad heredoc
libera l'array di comando e argomenti e le flag quotes
*/
void	execute_single_command(t_command *all_cmds,
			t_command *command, t_shell_state *shell_state)
{
	char			**args_array;
	bool			*args_quote_flags;
	int				is_builtin_cmd;
	t_redirection	*redirection;
	t_fork_info		fork_info;

	redirection = command->redirections;
	if (initialize_command_args(command, &args_array, &args_quote_flags) != 0)
		return (free_args_array(args_array));
	is_builtin_cmd = is_builtin(command->cmd_name);
	fork_info = (t_fork_info){command, args_array, args_quote_flags};
	if (is_builtin_cmd && command->next == NULL)
		exec_bltn_in_parent(command, args_array, args_quote_flags, shell_state);
	else
		handle_fork(all_cmds, &fork_info, shell_state);
	while (redirection)
	{
		if (redirection->type == TOKEN_HEREDOC)
			clean_filename(redirection);
		redirection = redirection->next;
	}
	free_args_array(args_array);
	free(args_quote_flags);
}
