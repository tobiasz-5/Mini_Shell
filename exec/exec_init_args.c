/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_init_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 10:07:28 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/27 19:23:46 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
inizializza il comando, chiamando prepare arguments
chiamante e' execute_single_command in exec.c
char ***args_array, passimo puntatore a doppio puntatore a char
bool **args_quote_flags, passiamo puntatore a puntatore a bool
[per modificare senza ritornare]
*/
int	initialize_command_args(t_command *command, char ***args_array,
				bool **args_quote_flags)
{
	if (prepare_arguments(command, args_array, args_quote_flags) != 0)
		return (-1);
	return (0);
}

/*
prepara gli argomenti
converte lista di argomenti in un array di stringhe**
[--passiamo triplo puntatore per aggiornare direttament puntatore
al doppio puntatore senza dover ritornarlo (aggiorna dinamicamente
array di stringhe, cosi modifiche restano tali anche fuori dalla f)
-> passaggio per riferimento, la f modifica direttamente l'array
per il chiamante, senza ritornarlo]
*/
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
