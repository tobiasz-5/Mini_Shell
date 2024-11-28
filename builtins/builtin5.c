/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 10:50:33 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/28 11:16:29 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
stampa le variabili exportate
nel formato richiesto da bash
se il valore della var e' null e la flag exported_empty e' vera
stampa solo il nome-> name=""
altrimenti stampa nome e valore name="val"
*/
void	print_exported_vars(t_env_var *env_list)
{
	while (env_list)
	{
		if (env_list->value == NULL && env_list->exported_empty)
			printf("declare -x %s\n", env_list->name);
		else
			printf("declare -x %s=\"%s\"\n", env_list->name, env_list->value);
		env_list = env_list->next;
	}
}

/*
gestisce/esegue comando export
se non ci sono argomenti per export-> stampa tutte le var nel formato richiesto
da bash (declare -x) con print_exported_vars
altrimenti cicla sugli argomenti e chiama handle_export_command per
exportare/aggiornare le var
*/
void	execute_export(char **args_array, bool *args_quote_flags,
				t_shell_state *shell_state)
{
	int		i;
	bool	quote_flag;

	if (args_array[1] == NULL)
		print_exported_vars(shell_state->env_list);
	else
	{
		i = 1;
		while (args_array[i] != NULL)
		{
			quote_flag = false;
			if (args_quote_flags != NULL)
				quote_flag = args_quote_flags[i];
			handle_export_command(&shell_state->env_list, args_array[i],
				quote_flag, false);
			i++;
		}
	}
}

/*
esegue i built-in implementati
confronta nome comando con stringhe dei builtin disponibili
-se cmd e' unset e non ci sono arg scrive mssg di errore
altrimenti cicla sugli arg di unset
*/
void	execute_builtin(t_command *command, char **args_array,
			bool *args_quote_flags, t_shell_state *shell_state)
{
	int	i;

	i = 0;
	if (ft_strcmp(command->cmd_name, "cd") == 0)
		execute_cd(args_array, shell_state);
	else if (ft_strcmp(command->cmd_name, "pwd") == 0)
		execute_pwd(shell_state);
	else if (ft_strcmp(command->cmd_name, "exit") == 0)
		execute_exit(args_array, shell_state);
	else if (ft_strcmp(command->cmd_name, "echo") == 0)
		execute_echo(args_array, shell_state);
	else if (ft_strcmp(command->cmd_name, "export") == 0)
		execute_export(args_array, args_quote_flags, shell_state);
	else if (ft_strcmp(command->cmd_name, "env") == 0)
		builtin_env(shell_state);
	else if (ft_strcmp(command->cmd_name, "unset") == 0)
	{
		if (args_array[1] == NULL)
			printf("unset: missing argument\n");
		else
		{
			while (args_array[i++])
				unset_env_var(&shell_state->env_list, args_array[i]);
		}
	}
}
