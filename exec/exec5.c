/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec5.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: negambar <negambar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:06:18 by negambar          #+#    #+#             */
/*   Updated: 2024/11/05 16:27:25 by negambar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

int	backup_fd(int old_fd, int *backup_fd, const char *error_msg)
{
	if (*backup_fd == -1)
	{
		*backup_fd = dup(old_fd);
		if (*backup_fd < 0)
		{
			perror(error_msg);
			return (-1);
		}
	}
	return (0);
}
