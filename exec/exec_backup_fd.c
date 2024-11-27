/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_backup_fd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 10:08:09 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/27 14:34:16 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

/*
esegue backup di un fd
-se backup non e' ancora stato effettuato (backup_fd = -1)
esegue dup2 per duplicare l fd, per poter ripristinare in seguito
*/
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
