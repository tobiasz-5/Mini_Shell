/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 16:46:13 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/25 12:58:13 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
riceve due puntatori a interi, li impost a zero
*/
void	init_dasvidaniya_norminette(int *len, int *i)
{
	*len = 0;
	*i = 0;
}

/*
copia valore stringa exit_status in resut,  e aggiorna indice/puntatore
*/
void	copy_exit_status(char *result, int *j, char *exit_status_str)
{
	int	k;

	k = 0;
	while (exit_status_str[k])
	{
		result[*j] = exit_status_str[k];
		k++;
		(*j)++;
	}
}
