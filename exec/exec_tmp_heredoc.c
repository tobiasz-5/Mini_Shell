/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tmp_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 10:06:23 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/27 10:54:59 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"
#define MAX_ATTEMPTS 100

/*
crea file temp unico per heredeco
duplica nome generato e lo assegna a 'heredoc_filename'
se qualcosa fallisce, chiude l'fd vars.fd,
rimuove file temporaneo con unlink, ritorna errore
altrimenti ritorna l'fd del file creato
*/
int	create_temp_heredoc_file(char **heredoc_filename)
{
	t_filename_vars	vars;

	if (generate_unique_filename(&vars) != 0)
	{
		*heredoc_filename = NULL;
		return (-1);
	}
	*heredoc_filename = ft_strdup(vars.tmp_filename);
	if (!*heredoc_filename)
	{
		perror("ft_strdup");
		close(vars.fd);
		unlink(vars.tmp_filename);
		*heredoc_filename = NULL;
		return (-1);
	}
	return (vars.fd);
}

/*
converte intero in stringa
se num e' zero assegna zero
altrimenti se negativo imposta flag e lo rende positivo
*/
void	init_int_to_str_vars(t_int_to_str_vars *vars, int num)
{
	vars->i = 0;
	vars->is_negative = 0;
	vars->temp_num = num;
	if (num == 0)
	{
		vars->temp_str[vars->i++] = '0';
		vars->temp_str[vars->i] = '\0';
	}
	else if (num < 0)
	{
		vars->is_negative = 1;
		vars->temp_num = -num;
	}
}

/*
converte intero in stringa
se num e' zero -> stringa e' zero + '\0'
altrimenti cicla riempiendo stringa dall'inizio
con l ultimo 'char' -> -123 -> 321-
->infine lo riordina -123
*/
void	int_to_str(int num, char *str)
{
	t_int_to_str_vars	vars;

	init_int_to_str_vars(&vars, num);
	if (num == 0)
	{
		str[0] = vars.temp_str[0];
		str[1] = '\0';
		return ;
	}
	while (vars.temp_num > 0)
	{
		vars.temp_str[vars.i++] = (vars.temp_num % 10) + '0';
		vars.temp_num /= 10;
	}
	if (vars.is_negative)
		vars.temp_str[vars.i++] = '-';
	vars.j = 0;
	while (vars.i > 0)
		str[vars.j++] = vars.temp_str[--vars.i];
	str[vars.j] = '\0';
}

/*
tenta fino a max_attempts volte di creare nome unico per il file
-usa int_to_str per aggiungere un identificatore unico al nome base
/tmp/heredoc_
-crea file con open
-se fd != -1 allora open file ha avuto successo, quindi esce dal
while, ritorna ok al chiamante
*/
int	generate_unique_filename(t_filename_vars *vars)
{
	vars->unique_id = 0;
	while (vars->unique_id < MAX_ATTEMPTS)
	{
		ft_strcpy(vars->tmp_filename, "/tmp/heredoc_");
		int_to_str(vars->unique_id, vars->unique_id_str);
		ft_strcat(vars->tmp_filename, vars->unique_id_str);
		vars->fd = open(vars->tmp_filename, O_CREAT | O_EXCL | O_RDWR, 0600);
		if (vars->fd != -1)
			return (0);
		if (errno != EEXIST)
		{
			perror("open");
			return (-1);
		}
		vars->unique_id++;
	}
	write(2, "Failed to create unique heredoc file\n", 37);
	return (-1);
}

/*
elimina file temporaneo dell heredoc
e libera la memoria allocate per heredoc_filename
*/
void	clean_filename(t_redirection *redirection)
{
	if (redirection->heredoc_filename)
	{
		unlink(redirection->heredoc_filename);
		free(redirection->heredoc_filename);
		redirection->heredoc_filename = NULL;
	}
}
