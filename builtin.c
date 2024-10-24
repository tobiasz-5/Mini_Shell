/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:45:20 by tschetti          #+#    #+#             */
/*   Updated: 2024/10/16 18:58:19 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

void	execute_cd(char **args_array, t_shell_state *shell_state)
{
	if (args_array[1] == NULL)
	{
		printf("cd: missing argument\n");
		shell_state->last_exit_status = 1;
	}
	else if (args_array[2] != NULL)
	{
		printf("bash: cd: too many arguments\n");
		shell_state->last_exit_status = 1;
	}
	else
	{
		if (chdir(args_array[1]) != 0)
		{
			perror("cd");
			shell_state->last_exit_status = 1;
		}
		else
		{
			shell_state->last_exit_status = 0;
		}
	}
}

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

int	parse_echo_options(char **args_array, int *newline)
{
	int	i;
	int	j;

	i = 1;
	*newline = 1;
	while (args_array[i])
	{
		j = 1;
		if ((args_array[i][0] == '-'
			|| (args_array[i][0] == '\'' && args_array[i][1] == '-')
				|| (args_array[i][0] == '\"' && args_array[i][1] == '-')))
		{
			while (args_array[i][j] == 'n' || args_array[i][j] == '\"'
				|| args_array[i][j] == '\'')
				j++;
			if (args_array[i][j] == '\0')
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
