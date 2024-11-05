/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_main_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: negambar <negambar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:42:46 by negambar          #+#    #+#             */
/*   Updated: 2024/11/05 16:46:14 by negambar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

void	update_last_exit_status(t_shell_state *shell_state)
{
	if (g_received_signal)
	{
		shell_state->last_exit_status = 128 + g_received_signal;
		g_received_signal = 0;
	}
} 