/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_env_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 10:07:51 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/27 14:07:46 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
conta numero var in env_list
*/
int	count_env_vars(t_env_var *env_list)
{
	int			count;
	t_env_var	*current;

	current = env_list;
	count = 0;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

/*
crea stringa fomrattata nome=valore
-calcola le len e le somma(+2per '=' e '\0'), alloca mem
per la stringa concatenata e copia nella stringa
allocata, poi la ritorna
*/
char	*create_env_var_string(t_env_var *env_var)
{
	size_t	name_len;
	size_t	value_len;
	size_t	len;
	char	*env_string;

	name_len = ft_strlen(env_var->name);
	if (env_var->value)
		value_len = strlen(env_var->value);
	else
		value_len = 0;
	len = name_len + 1 + value_len + 1;
	env_string = malloc(len);
	if (!env_string)
		return (NULL);
	memcpy(env_string, env_var->name, name_len);
	env_string[name_len] = '=';
	if (value_len > 0)
		memcpy(env_string + name_len + 1, env_var->value, value_len);
	env_string[name_len + 1 + value_len] = '\0';
	return (env_string);
}

/*
libera l'array di stringhe envp in caso di errore
*/
void	free_envp_on_error(char **envp, int i)
{
	while (i > 0)
	{
		i--;
		free(envp[i]);
	}
	free(envp);
}

/*
crea array di string envp dalla lista di var d'ambiente
chiama le f viste sopra
->per ogni nodo/var d'ambiente crea stringa
nome=valore e la mette nell array che poi
alla fine ritorna
*/
char	**build_envp(t_shell_state *shell_state)
{
	int			count;
	char		**envp;
	t_env_var	*current;
	int			i;

	count = count_env_vars(shell_state->env_list);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	current = shell_state->env_list;
	i = 0;
	while (current)
	{
		envp[i] = create_env_var_string(current);
		if (!envp[i])
		{
			free_envp_on_error(envp, i);
			return (NULL);
		}
		i++;
		current = current->next;
	}
	envp[i] = NULL;
	return (envp);
}
