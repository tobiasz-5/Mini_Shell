/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 18:59:40 by tschetti          #+#    #+#             */
/*   Updated: 2024/10/16 18:59:41 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

int	validate_exit_code(char **args_array, t_shell_state *shell_state)
{
	int	i;

	i = 0;
	while (args_array[1][i])
	{
		if (!ft_isdigit(args_array[1][i]) && args_array[1][i] != '-')
		{
			printf("Mini exit: %s num required\n", args_array[1]);
			shell_state->exit_shell = true;
			shell_state->exit_code = 2;
			return (-1);
		}
		i++;
	}
	if (args_array[2] != NULL)
	{
		printf("bash: exit: too many arguments\n");
		shell_state->last_exit_status = 1;
		return (-1);
	}
	return (ft_atoi(args_array[1]) % 256);
}

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
