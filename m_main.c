/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:46:27 by tschetti          #+#    #+#             */
/*   Updated: 2024/10/06 22:46:27 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

void	init_env_list(t_shell_state *shell_state)
{
	int				i;
	char			*env_var;
	t_env_var		*new_var;
	char			*equal_sign;

	i = 0;
	while (shell_state->envp[i])
	{
		env_var = shell_state->envp[i];
		equal_sign = ft_strchr(env_var, '=');
		if (equal_sign)
		{
			new_var = malloc(sizeof(t_env_var));
			if (!new_var)
				return ;
			new_var->name = ft_strndup(env_var, equal_sign - env_var);
			new_var->value = ft_strdup(equal_sign + 1);
			new_var->next = shell_state->env_list;
			shell_state->env_list = new_var;
		}
		i++;
	}
}

void	clean_shell_state(t_shell_state *shell_state)
{
	t_env_var	*current;
	t_env_var	*next;

	current = shell_state->env_list;
	while (current)
	{
		next = current->next;
		free(current->name);
		free(current->value);
		free(current);
		current = next;
	}
}

void	shell_loop(char **envp)
{
	char			*input;
	t_shell_state	shell_state;

	shell_state.last_exit_status = 0;
	shell_state.env_list = NULL;
	shell_state.envp = envp;
	shell_state.exit_shell = false;
	shell_state.exit_code = 0;
	init_env_list(&shell_state);
	while (!shell_state.exit_shell)
	{
		input = readline(COLOR_BLUE"MINIPROMPT$ "COLOR_RESET);
		if (!input)
			break ;
		else if (*input == '\0')
		{
			free(input);
			continue ;
		}
		add_history(input);
		process_input(input, &shell_state);
		free(input);
		restore_signals_after_command();
		// if (g_received_signal != 0)//questo forse e' inutile
		// {
		// 	g_received_signal = 0;
		// }
		if (shell_state.exit_shell)
			break ;
	}
	rl_clear_history();
	clean_shell_state(&shell_state);
	exit(shell_state.exit_code);
}

int	main(int ac, char **av, char **envp)
{
	if (ac > 1)
		return (printf("Usage: %s\t [No Additional Arguments]\n", av[0]), 1);
	init_sign();
	shell_loop(envp);
}
