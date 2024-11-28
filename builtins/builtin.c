/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 10:50:09 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/28 16:01:01 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
gestisce errore caso var HOME non impostata
*/
void	handle_home_error(t_shell_state *shell_state)
{
	printf("cd: HOME non impostata\n");
	shell_state->last_exit_status = 1;
}

/*
esegue cd
-se non ci sono argomenti ottiene la var HOME
attraverso getenv e la assegna a dir,
se dir/HOME non e' impostata gestisce l'errore
-se ha piu di un argomento, stampa errore, imposta exit_status
e ritorna
-altrimenti la dir su cui "andare" e' il primo arg
-la f chdir ci porta alla dir, se fallisce stampa errore e aggiorna
last_exit_status
*/
void	execute_cd(char **args_array, t_shell_state *shell_state)
{
	char	*dir;

	if (args_array[1] == NULL)
	{
		dir = getenv("HOME");
		if (dir == NULL)
			return (handle_home_error(shell_state));
	}
	else if (args_array[2] != NULL)
	{
		printf("bash: cd: too many arguments\n");
		shell_state->last_exit_status = 1;
		return ;
	}
	else
		dir = args_array[1];
	if (chdir(dir) != 0)
	{
		write(2, "bash: cd: ", 10);
		write(2, dir, ft_strlen(dir));
		perror(" ");
		shell_state->last_exit_status = 1;
	}
	else
		shell_state->last_exit_status = 0;
}

/*
esegue pwd
chiama getcwd per ottenere la dir corrente
se tutto ok imposta exit_status a 0
se getcwd fallisce,(ritorna null) chiama perror
imposta exit_status a 1
*/
void	execute_pwd(t_shell_state *shell_state)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		shell_state->last_exit_status = 0;
	}
	else
	{
		perror("pwd");
		shell_state->last_exit_status = 1;
	}
}

/*
analizza opzione di echo, -n
i = 1 per saltare echo
flag new_line impostata a true di default
cicla sugli argomeni, j = 1 per saltare
' o " o -
-poi controlla: se l opzione inizia con - 
o inizia con ' ed e' seguita da -
o inizia con " ed e' seguita da -
e che carattere dopo - sia n
-poi cicla finche ci sono n o ' o "
j++
-se l opzione termina correttamente
imposta flag a false 0 e passa
all arg successivo (perche si puo avere -n -n -nnnn in bash)
*/
int	parse_echo_options(char **array, int *newline)
{
	int	i;
	int	j;

	i = 1;
	*newline = 1;
	while (array[i])
	{
		j = 1;
		if ((array[i][0] == '-' || (array[i][0] == '\'' && array[i][1] == '-')
		|| (array[i][0] == '\"' && array[i][1] == '-'))
		&& array[i][1] == 'n')
		{
			while (array[i][j] == 'n' || array[i][j] == '\"'
				|| array[i][j] == '\'')
				j++;
			if (array[i][j] == '\0')
			{
				*newline = 0;
				i++;
				continue ;
			}
		}
		break ;
	}
	return (i);
}

/*
esegue echo
-analizza opzione -n e ritorna indice/posizione
primo argomento
-cicla stampando ogni arg, se c'e un successivo arg
stampa anche uno spazio
-se il flag newline e' true stampa anche '\n'
*/
void	execute_echo(char **args_array, t_shell_state *shell_state)
{
	int	i;
	int	newline;

	i = parse_echo_options(args_array, &newline);
	while (args_array[i])
	{
		printf("%s", args_array[i]);
		i++;
		if (args_array[i])
			printf(" ");
	}
	if (newline)
		printf("\n");
	shell_state->last_exit_status = 0;
}
