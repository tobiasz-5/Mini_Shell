/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 10:08:03 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/27 20:52:20 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
finalizza redir input
-se last_input_fd e' stato usato/aperto
duplica, con dup2, l'fd su stdin
-poi chiude l'fd perche non serve piu e va chiuso
*/
void	finalize_input_redirection(int last_input_fd)
{
	if (last_input_fd != -1)
	{
		if (dup2(last_input_fd, STDIN_FILENO) < 0)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(last_input_fd);
	}
}

/*
apre file in sola lettura
se open fallisce (val < 0), chiama perror e ritorna -999 al chiamante
process_input_redirection, se open e' ok la f restituisce
il ritorno di open(cioe' valore intero >= a zero [0,1 e 2 sono riservati
rispettiv. a stdin,stdout, stderr])
*/
int	open_input_file(const char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("Error opening input file");
		return (-999);
	}
	return (fd);
}

/*
processa redir di input
-chiude file di input se e' stato aperto 
in precedenza-> valore diverso da -1(val di default)
-gestisce sia redir in che heredoc:
se redir in usa direttamente filename se heredoc
usa nome file heredoc tmp generato prima /tmp/heredoc_ ecc.
-apre l'fd last_input_fd(solo lettura)
-se errore di apertura (-999 err custom) ritorna -999
e lo assegna a last_input_fd, quindi se c'e' un errore
nell open -> last_input_fd e' -999 [fd validi possono essere solo 0 o positivi]
(e quindi ritorna 'errore' return 0)
altrimenti open_input_file assegna a last_input_fd il ritorno di open
e questa f ritorna 1->ok
*/
int	process_input_redirection(t_redirection *redirection,
			int *last_input_fd)
{
	const char	*filename;

	filename = NULL;
	if (*last_input_fd != -1)
		close(*last_input_fd);
	if (redirection->type == TOKEN_REDIR_IN)
		filename = redirection->filename;
	else if (redirection->type == TOKEN_HEREDOC)
	{
		if (redirection->heredoc_filename == NULL)
		{
			write(2, "Errorheoc_filme NULL hdle_child_redions\n", 41);
			exit(EXIT_FAILURE);
		}
		filename = redirection->heredoc_filename;
	}
	else
		return (1);
	*last_input_fd = open_input_file(filename);
	if (*last_input_fd == -999)
		return (0);
	return (1);
}

/*
processa redir di input primo if
o altrimenti redir di output secondo if
*/
int	process_redirection(t_redirection *redirection,
			int *last_input_fd, t_io_fds *fds, t_shell_state *shell_state)
{
	if (redirection->type == TOKEN_REDIR_IN
		|| redirection->type == TOKEN_HEREDOC)
	{
		if (!process_input_redirection(redirection, last_input_fd))
			return (0);
	}
	else if (redirection->type == TOKEN_REDIR_OUT
		|| redirection->type == TOKEN_REDIR_APPEND)
	{
		if (apply_redirection(redirection, fds, shell_state) < 0)
		{
			exit(EXIT_FAILURE);
		}
	}
	return (1);
}

/*
gestisce le redir nel figlio
-inizializza last_input_fd a -1
-cicla su tutte le redir del comando
-processa i vari tipi di redir
-finalizza la redir di input
*/
int	handle_child_redirections(t_command *command,
			t_io_fds *fds, t_shell_state *shell_state)
{
	t_redirection	*redirection;
	int				last_input_fd;

	redirection = command->redirections;
	last_input_fd = -1;
	while (redirection)
	{
		if (!process_redirection(redirection, &last_input_fd, fds, shell_state))
			return (0);
		redirection = redirection->next;
	}
	finalize_input_redirection(last_input_fd);
	return (1);
}
