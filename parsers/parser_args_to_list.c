/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_args_to_list.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 18:34:47 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/26 12:27:57 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
conta numero di argomenti in arg_list
ritorna il numero totale di argomenti
*/
int	count_arguments(t_list_args *args_list)
{
	int	count;

	count = 0;
	while (args_list)
	{
		count++;
		args_list = args_list->next;
	}
	return (count);
}

/*
controlla che args_array si stato allocato correttamente o
che ci sia almeno un argomento quando allochiamo array per
args_quote_flags -> libera sempre args_array, libera 
anche quote_flags se count maggio di zero (funzione ridondante,
sotto e' ok credo)
*/
int	check_and_free_memory(char **args_array, bool *args_quote_flags, int count)
{
	if (!args_array || (count > 0 && !args_quote_flags))
	{
		free(args_array);
		if (count > 0)
			free(args_quote_flags);
		return (0);
	}
	return (1);
}

// int	check_and_free_memory(char **args_array, bool *args_quote_flags)
// {
// 	if (!args_array || !args_quote_flags)
// 	{
// 		free(args_array);
// 		free(args_quote_flags);
// 		return (0);
// 	}
// 	return (1);
// }

/*
Popola args_array e args_quote_flags -> l'array di argomenti e
l'array di flag bool per le virgolette
popola l'array** di argomenti a partire dal campo arg dell lista di comandi
args_list
imposta true o false per ogni argomento in base all pres o meno di
quotes, se c'e un errore libera cio che era stato allocato
e restituisce false altrimenti true
*/
bool	populate_args_arrays(t_list_args *args_list,
				char **args_array, bool *args_quote_flags)
{
	t_list_args	*arg;
	int			i;

	arg = args_list;
	i = 0;
	while (arg)
	{
		args_array[i] = ft_strdup(arg->arg);
		if (!args_array[i])
		{
			while (i > 0)
			{
				i--;
				free(args_array[i]);
			}
			return (false);
		}
		args_quote_flags[i] = arg->single_quote || arg->double_quote;
		arg = arg->next;
		i++;
	}
	return (true);
}

/*
converte lista di argomenti args_list in array
alloca memoria per array di stringhe ** args_array
e per bool per le quotes * arg_quote_flags
-> controllo errori e liberazioni
-> popola gli array
-> assegna a args_quote_flags_ptr un bool per indicare se
ci sono virgolette, che siano '' o ""
ritorna l'array di stringhe args_array -->array di argomenti
[incluso il primo]->[es. (1)'echo' (2)'-n' (3)'ciao'],
inoltre ogni argomento ha un array di bool flag associato
'[bool], [bool], [bool]' che indica se ha una qualche virgoletta
*/
char	**convert_arglist_for_exc(t_command *command,
				bool **args_quote_flags_ptr)
{
	char		**args_array;
	bool		*args_quote_flags;
	int			count;

	count = count_arguments(command->args_list);
	args_array = malloc(sizeof(char *) * (count + 1));
	if (count > 0)
		args_quote_flags = malloc(sizeof(bool) * count);
	else
		args_quote_flags = NULL;
	if (!check_and_free_memory(args_array, args_quote_flags, count))
		return (NULL);
	if (!populate_args_arrays(command->args_list,
			args_array, args_quote_flags))
	{
		free(args_array);
		free(args_quote_flags);
		return (NULL);
	}
	args_array[count] = NULL;
	*args_quote_flags_ptr = args_quote_flags;
	return (args_array);
}
