/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 10:50:20 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/28 13:10:34 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
verifica se e' un nome di variabile valido
-deve iniziare con lettere o _
-poi puo contenere solo lettere, numeri o _
*/
bool	is_valid_var_name(const char *name)
{
	size_t	i;

	if (!name || !name[0])
		return (false);
	if (!(ft_isalpha(name[0]) || name[0] == '_'))
		return (false);
	i = 1;
	while (name[i])
	{
		if (!(ft_isalnum(name[i]) || name[i] == '_'))
			return (false);
		i++;
	}
	return (true);
}

/*
libera nome, valore e struttura
*/
void	free_env_var(t_env_var *env_var)
{
	if (env_var)
	{
		free(env_var->name);
		free(env_var->value);
		free(env_var);
	}
}

/*
unsetta/rimuove una var d'ambiente,
cerca la var nella lista, se la trova la rimuove
-imposta nodo precedente a null all inizio
-**env_list doppio p per modificare direttamente nel chiamante
-se esiste un nodo precedente-> prev->next = current->next
collega il precedente al successivo, perche il corrente viene rimosso
poco dopo
altrimenti aggiorna testa lista *env_list = current->next
*/
void	unset_env_var(t_env_var **env_list, const char *name)
{
	t_env_var	*current;
	t_env_var	*prev;

	if (env_list == NULL || *env_list == NULL || name == NULL)
		return ;
	current = *env_list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env_list = current->next;
			free_env_var(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

/*
stampa le var di ambiente con env
-verifica se puntatore value esiste e se
valore puntato da value e' non vuoto
->stampa le var che soddisfano l if
*/
void	print_env_vars(t_env_var *env_list)
{
	while (env_list)
	{
		if (env_list->value != NULL && env_list->value[0] != '\0')
			printf("%s=%s\n", env_list->name, env_list->value);
		env_list = env_list->next;
	}
}

/*
esegue env
chiama la f per stampare le var appropriate
*/
void	builtin_env(t_shell_state *shell_state)
{
	print_env_vars(shell_state->env_list);
}
