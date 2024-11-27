/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_init_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 10:07:28 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/27 10:07:30 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

int	initialize_command_args(t_command *command, char ***args_array,
				bool **args_quote_flags)
{
	if (prepare_arguments(command, args_array, args_quote_flags) != 0)
		return (-1);
	return (0);
}

int	prepare_arguments(t_command *command, char ***args_array,
				bool **args_quote_flags)
{
	*args_array = convert_arglist_for_exc(command, args_quote_flags);
	if (!*args_array)
	{
		perror("convert_arglist_for_exc");
		return (-1);
	}
	return (0);
}
