/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_process_heredoc.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 18:54:53 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/27 18:54:56 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"
#define MAX_LINE_LENGTH 4096

/*
gestisce loop per heredoc
mostra un prompt e legge la linea d'input
->booleano per uscire o sigint ricevuto
anche delimiter
->espande se necessario e scrive nel file
associato
*/
void	heredoc_input_loop(const char	*delimiter, int fd,
			bool is_quoted, t_shell_state *shell_state)
{
	char	line[MAX_LINE_LENGTH];
	size_t	line_len;
	bool	should_exit;

	should_exit = false;
	while (1)
	{
		write(1, "> ", 2);
		should_exit = read_input_line(line, &line_len);
		if (should_exit || g_received_signal == SIGINT)
		{
			write(1, "\n", 1);
			break ;
		}
		if (strcmp(line, delimiter) == 0)
			break ;
		expand_and_write_line(line, fd, is_quoted, shell_state);
	}
}

/*
elabora tutti gli heredoc associati ai comandi
-chiama process_command_heredocs per elaborare
gli heredoc di ogni comando, se fallisce ritorna 1
*/
int	process_all_heredocs(t_command *command_list, t_shell_state *shell_state)
{
	t_command		*current_cmd;

	current_cmd = command_list;
	while (current_cmd)
	{
		if (process_command_heredocs(current_cmd, shell_state) < 0)
			return (1);
		current_cmd = current_cmd->next;
	}
	return (0);
}

/*
elabora heredocs per un comando
-scorre la lista delle redirezioni del comando
per ogni tipo heredoc che trova
-chiama handle_heredoc, che crea file temporaneo,
legge l input e se necessario lo espande
salva il nome del file nel campo heredoc_filename
della struttura redirection
*/
int	process_command_heredocs(t_command *current_cmd, t_shell_state *shell_state)
{
	t_redirection	*redirection;
	char			*heredoc_filename;

	redirection = current_cmd->redirections;
	while (redirection)
	{
		if (redirection->type == TOKEN_HEREDOC)
		{
			heredoc_filename = NULL;
			handle_heredoc(redirection, &heredoc_filename, shell_state);
			if (!heredoc_filename)
			{
				write(2, "failed to create heredoc file 4 redir\n", 38);
				return (-1);
			}
			redirection->heredoc_filename = heredoc_filename;
		}
		redirection = redirection->next;
	}
	return (0);
}

/*
gestisce un singolo heredoc
-crea un file temporaneo,
-legge ed espande contenuto dell heredoc
*/
void	handle_heredoc(t_redirection *redirection,
			char **heredoc_filename, t_shell_state *shell_state)
{
	int	fd;

	fd = create_temp_heredoc_file(heredoc_filename);
	if (fd == -1)
		return ;
	read_and_expand_heredoc(redirection->filename, fd,
		redirection->is_quoted, shell_state);
	close(fd);
}

/*
legge ed espande contenuto heredoc
configura gestore segnali
-sigint per interrompere mod heredoc
-sigquit ignorato
chiama heredoc_input_loop per leggere le linee di input
ripristina i gestori dei segnali originali alla fine del loop
*/
void	read_and_expand_heredoc(const char *delimiter, int fd,
				bool is_quoted, t_shell_state *shell_state)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;
	struct sigaction	sa_old_int;
	struct sigaction	sa_old_quit;

	sa_int.sa_handler = handle_sigint_heredoc;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, &sa_old_int);
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, &sa_old_quit);
	heredoc_input_loop(delimiter, fd, is_quoted, shell_state);
	sigaction(SIGINT, &sa_old_int, NULL);
	sigaction(SIGQUIT, &sa_old_quit, NULL);
}
