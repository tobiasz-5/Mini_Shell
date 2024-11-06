/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander2_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: negambar <negambar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:46:00 by girindi          #+#    #+#             */
/*   Updated: 2024/11/05 16:16:39 by negambar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

void	init_dasvidaniya_norminette(int *len, int *i)
{
	*len = 0;
	*i = 0;
}

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
