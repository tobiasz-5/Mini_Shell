/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 15:42:52 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/26 16:36:11 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
inizializza i file descriptors della struttura t_io_fds
tutti a -1-> non sono ancora utilizzati
include i backup di stdin e stdout per ripristino dopo
redir
*/
void	init_io_fds(t_io_fds *fds)
{
	fds->infile_fd = -1;
	fds->outfile_fd = -1;
	fds->heredoc_fd = -1;
	fds->stdin_backup = -1;
	fds->stdout_backup = -1;
}

/*
ripristina gli fds a partire dai backup [stdin,stdout]
se i backup sono validi/stati utilizzati -> sono diversi da -1
dunque li ripristina usando dup2, poi chiude i vecchi fd per 
i backup e ripristina il valore di default -1 per gli fd
che rappresentano gli fd di backup
*/
void	restore_standard_fds(t_io_fds *fds)
{
	if (fds->stdin_backup != -1)
	{
		if (dup2(fds->stdin_backup, STDIN_FILENO) < 0)
		{
			perror("Error restoring stdin");
			close(fds->stdin_backup);
			fds->stdin_backup = -1;
			return ;
		}
		close(fds->stdin_backup);
		fds->stdin_backup = -1;
	}
	if (fds->stdout_backup != -1)
	{
		if (dup2(fds->stdout_backup, STDOUT_FILENO) < 0)
		{
			perror("Error restoring stdout");
			close(fds->stdout_backup);
			fds->stdout_backup = -1;
			return ;
		}
		close(fds->stdout_backup);
		fds->stdout_backup = -1;
	}
}

/*
esegue comando built-in
*/
void	execute_builtin_command(t_command *command, char **args_array,
				bool *args_quote_flags, t_shell_state *shell_state)
{
	execute_builtin(command, args_array, args_quote_flags, shell_state);
	shell_state->last_exit_status = 0;
}

/*
gestisce errori relativa alla creazione dell array di
variabili d'ambiente envp
Libera le risorse associate:
tutti i comandi, gli argomenti, l'array envp
esce con exit_failure->=1
*/
void	handle_envp_error(t_command *all_cmds, char **args_array, char **envp)
{
	perror("build_envp");
	free_command_list(all_cmds);
	free_args_array(args_array);
	free_args_array(envp);
	exit(EXIT_FAILURE);
}

/*
esegue comando esterno
execve, eseguibile
->costruisce envp**
->determina percorso completo del comando
->pulisce lo stato della shell
->se il percorso non viene creato correttamente:
stampa messaggio di errore: command not found: nome del comando
e libera le risorse utilizzate, esce con 127->cmd not found
altimenti esegue il comando
le righe successive vengono eseguite solo se execve ritorna
valore negativo a indicare errore
execve "sostituisce/carica" il processo corrente con il nuovo programma
eseguibile-->>i.e. il processo in corso carica l esegubile e
inizia ad eseguirlo, da qui in poi lo sostituisce del tutto
*/
void	exec_ext_cmd(t_command *all_cmds, t_command *command,
				char **args_array, t_shell_state *shell_state)
{
	char	*full_path;
	char	**envp;

	envp = build_envp(shell_state);
	if (!envp)
		handle_envp_error(all_cmds, args_array, envp);
	full_path = get_command_path(command->cmd_name, shell_state);
	clean_shell_state(shell_state);
	shell_state->env_list = NULL;
	if (!full_path)
	{
		printf("Command not found: %s\n", command->cmd_name);
		free_command_list(all_cmds);
		free_args_array(args_array);
		free_args_array(envp);
		clean_shell_state(shell_state);
		exit(127);
	}
	execve(full_path, args_array, envp);
	perror("execve");
	free(full_path);
	free_command_list(all_cmds);
	free_args_array(args_array);
	free_args_array(envp);
	exit(EXIT_FAILURE);
}
