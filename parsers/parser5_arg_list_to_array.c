/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser5_arg_list_to_array.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: negambar <negambar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:46:51 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/05 16:17:16 by negambar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

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
