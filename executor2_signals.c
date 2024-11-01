/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2_signals.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 23:42:51 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/01 12:27:11 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

int	initialize_command_args(t_command *command, char ***args_array,
				bool **args_quote_flags)
{
	if (prepare_arguments(command, args_array, args_quote_flags) != 0)
		return (-1);
	return (0);
}

void	handle_signals_in_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	restore_signals_after_command(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = handle_sigquit;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	handle_signals_in_parent(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void handle_fork(t_command* all_cmds, t_fork_info* finfo, t_shell_state* shell_state)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		handle_signals_in_child();
		exec_in_child(all_cmds, finfo, shell_state);
	}
	else if (pid > 0)
	{
		handle_signals_in_parent();
		handle_parent_after_fork(pid, shell_state);
		restore_signals_after_command();
	}
	else
	{
		perror("fork");
		shell_state->last_exit_status = 1;
	}
}
