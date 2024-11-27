/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 18:54:09 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/27 18:54:11 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
gestisce redir per i built-in
-scorre la lista delle redir per il cmd corrente
-applica le redir con apply_redirection
*/
int	handle_builtin_redirections(t_command *command, t_io_fds *fds,
					t_shell_state *shell_state)
{
	t_redirection	*redirection;

	redirection = command->redirections;
	while (redirection)
	{
		if (apply_redirection(redirection, fds, shell_state) < 0)
			return (-1);
		redirection = redirection->next;
	}
	return (0);
}

/*
gestisce redirezione input
-esegue back_up di stdin per poterlo ripristinare dopo
-apre file specificato dalla redirezione, mod solo lettura
-se errore di open, ritorna -1
-duplica fd aperto su stdin con dup2
-chiude l'fd aperto, non serve piu
*/
int	redirect_input(t_redirection *redirection, t_io_fds *fds)
{
	if (backup_fd(STDIN_FILENO, &fds->stdin_backup, "Err backing up stdin") < 0)
		return (-1);
	fds->infile_fd = open(redirection->filename, O_RDONLY);
	if (fds->infile_fd < 0)
	{
		perror("Error opening input file");
		return (-1);
	}
	if (dup2(fds->infile_fd, STDIN_FILENO) < 0)
	{
		perror("Error redirecting stdin");
		close(fds->infile_fd);
		return (-1);
	}
	close(fds->infile_fd);
	return (0);
}

/*
gestisce redir dell output
-in base al tipo > o >> sovrascrive o aggiunge
-esegue backup di stdout per ripristino
-apre il file con la flag >(trunc) o >>(append) appropriata per tipo di file
-se open da errore ritorna -1
-duplica fd aperto su stdout
-chiude l fd, non serve piu
*/
int	redirect_output(t_redirection *redirection, t_io_fds *fds)
{
	int	flags;

	if (redirection->type == TOKEN_REDIR_OUT)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else if (redirection->type == TOKEN_REDIR_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		return (-1);
	if (backup_fd(STDOUT_FILENO, &fds->stdout_backup,
			"Error backing up stdout") < 0)
		return (-1);
	fds->outfile_fd = open(redirection->filename, flags, 0644);
	if (fds->outfile_fd < 0)
	{
		perror("Error opening output file");
		return (-1);
	}
	if (dup2(fds->outfile_fd, STDOUT_FILENO) < 0)
	{
		perror("Error redirecting stdout");
		close(fds->outfile_fd);
		return (-1);
	}
	close(fds->outfile_fd);
	return (0);
}

/*
gestisce redir heredoc (f non usata-> vedi apply_heredoc_file
in exec_apply.c)
-apre file temp creato per heredoc (onlyread-> 1 solo lato lett.)
-se open da errore, perror, e ritorna -1
-chiama dup2 per duplicare l'fd per l'heredoc su stdin
-chiude fd per heredoc e cancella file tmp(unlink)
*/
int	perform_heredoc_redirection(const char *heredoc_filename)
{
	int	heredoc_fd;

	heredoc_fd = open(heredoc_filename, O_RDONLY);
	if (heredoc_fd < 0)
	{
		perror ("error opening heredoc file");
		return (-1);
	}
	if (dup2(heredoc_fd, STDIN_FILENO) < 0)
	{
		perror("error redirecting stdin for heredoc");
		close(heredoc_fd);
		return (-1);
	}
	close (heredoc_fd);
	unlink(heredoc_filename);
	return (0);
}

/*
gestisce redir heredoc -(questa f non viene usata)-> 
il suo sostituto e' in exec_apply.c ->apply_heredoc_file
-esegue backup_fd per ripristinare stdin successivamente
-genera e gestisce file tmp per heredoc tramite handle_heredoc
-duplica fd su stdin tramite perform_heredoc_redirection
*/
int	handle_heredoc_redirection(t_redirection *redirection, t_io_fds *fds,
			t_shell_state *shell_state)
{
	char	*heredoc_filename;

	heredoc_filename = NULL;
	if (backup_fd(STDIN_FILENO, &fds->stdin_backup,
			"Error backing up stdin for heredoc") < 0)
		return (-1);
	handle_heredoc(redirection, &heredoc_filename, shell_state);
	if (heredoc_filename == NULL)
	{
		write(2, "Error handling heredoc\n", 23);
		return (-1);
	}
	if (perform_heredoc_redirection(heredoc_filename) < 0)
	{
		free(heredoc_filename);
		return (-1);
	}
	free(heredoc_filename);
	return (0);
}
