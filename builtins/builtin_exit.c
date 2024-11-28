/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 10:50:01 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/28 13:41:59 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
da ascii a long
*/
static long	ft_atol(const char *s)
{
	long	res;
	int		sign;

	res = 0;
	sign = 1;
	while (*s == ' ' || (*s > 8 && *s < 14))
		s++;
	if (*s == '-')
		sign = -1;
	if (*s == '-' || *s == '+')
		s++;
	while (ft_isdigit(*s))
		res = res * 10 + *s++ - '0';
	return (res * sign);
}

/*
gestisce errori per argomento di exit non valid
-stamp msg custom
-aggiorna exit_code con codice 2
*/
int	handle_exit_error(char **args_array, t_shell_state *shell_state)
{
	printf("Mini exit: %s num required\n", args_array[1]);
	shell_state->exit_shell = true;
	shell_state->exit_code = 2;
	return (2);
}

/*
gestisce exit
-controlla argomento di exit sia valido
-controlla sia compreso tra int_min e int_max
-se c'e piu di un arg, errore
-ritorna con atoi codice di uscita attraverso modulo 256,
*/
int	validate_exit_code(char **args_array, t_shell_state *shell_state)
{
	int		i;
	long	exit_value;

	i = 0;
	while (args_array[1][i])
	{
		if (!ft_isdigit(args_array[1][i]) && args_array[1][i] != '-')
			return (handle_exit_error(args_array, shell_state));
		i++;
	}
	exit_value = ft_atol(args_array[1]);
	if (exit_value > INT_MAX || exit_value < INT_MIN)
	{
		printf("Mini exit: %s: numeric argument required\n", args_array[1]);
		shell_state->exit_shell = true;
		shell_state->exit_code = 2;
		return (-1);
	}
	if (args_array[2] != NULL)
	{
		printf("bash: exit: too many arguments\n");
		shell_state->last_exit_status = 1;
		return (-1);
	}
	return (ft_atoi(args_array[1]) % 256);
}

/*
esegue exit
-stampa sempre exit
-se non ci sono arg per exit, esce con codice 0
-altrimenti chiama validate_exit_code per gestire
argomenti di exit, poi esce dalla shell impostando la flag
a true, altrimenti se piu di un arg per exit
[if (exit_code == -1)] ritorna al chiamante
*/
void	execute_exit(char **args_array, t_shell_state *shell_state)
{
	int	exit_code;

	printf("exit\n");
	if (args_array[1] == NULL)
	{
		shell_state->exit_shell = true;
		shell_state->exit_code = 0;
		return ;
	}
	exit_code = validate_exit_code(args_array, shell_state);
	if (exit_code == -1)
		return ;
	shell_state->exit_shell = true;
	shell_state->exit_code = exit_code;
}
