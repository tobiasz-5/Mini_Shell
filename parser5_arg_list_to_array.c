/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser5_arg_list_to_array.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:46:51 by tschetti          #+#    #+#             */
/*   Updated: 2024/10/10 17:55:52 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

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

char	**convert_arglist_for_exc(t_command *command,
				bool **args_quote_flags_ptr)
{
	t_list_args	*arg;
	char		**args_array;
	bool		*args_quote_flags;
	int			i;
	int			count;

	count = count_arguments(command->args_list);
	args_array = malloc(sizeof(char *) * (count + 1));
	if (count > 0)
		args_quote_flags = malloc(sizeof(bool) * count);
	else
		args_quote_flags = NULL;
	if (!check_and_free_memory(args_array, args_quote_flags, count))
		return (NULL);
	arg = command->args_list;
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
			free(args_array);
			free(args_quote_flags);
			return (NULL);
		}
		args_quote_flags[i] = arg->single_quote || arg->double_quote;
		arg = arg->next;
		i++;
	}
	args_array[i] = NULL;
	*args_quote_flags_ptr = args_quote_flags;
	return (args_array);
}
