/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tmp_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girindi <girindi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:19:56 by girindi           #+#    #+#             */
/*   Updated: 2024/11/05 16:42:46 by girindi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"
#define MAX_ATTEMPTS 100

int	create_temp_heredoc_file(char **heredoc_filename)
{
	t_filename_vars	vars;

	if (generate_unique_filename(&vars) != 0)
	{
		*heredoc_filename = NULL;
		return (-1);
	}
	*heredoc_filename = ft_strdup(vars.tmp_filename);
	if (!*heredoc_filename)
	{
		perror("ft_strdup");
		close(vars.fd);
		unlink(vars.tmp_filename);
		*heredoc_filename = NULL;
		return (-1);
	}
	return (vars.fd);
}

void	init_int_to_str_vars(t_int_to_str_vars *vars, int num)
{
	vars->i = 0;
	vars->is_negative = 0;
	vars->temp_num = num;
	if (num == 0)
	{
		vars->temp_str[vars->i++] = '0';
		vars->temp_str[vars->i] = '\0';
	}
	else if (num < 0)
	{
		vars->is_negative = 1;
		vars->temp_num = -num;
	}
}

void	int_to_str(int num, char *str)
{
	t_int_to_str_vars	vars;

	init_int_to_str_vars(&vars, num);
	if (num == 0)
	{
		str[0] = vars.temp_str[0];
		str[1] = '\0';
		return ;
	}
	while (vars.temp_num > 0)
	{
		vars.temp_str[vars.i++] = (vars.temp_num % 10) + '0';
		vars.temp_num /= 10;
	}
	if (vars.is_negative)
		vars.temp_str[vars.i++] = '-';
	vars.j = 0;
	while (vars.i > 0)
		str[vars.j++] = vars.temp_str[--vars.i];
	str[vars.j] = '\0';
}

int	generate_unique_filename(t_filename_vars *vars)
{
	vars->unique_id = 0;
	while (vars->unique_id < MAX_ATTEMPTS)
	{
		ft_strcpy(vars->tmp_filename, "/tmp/heredoc_");
		int_to_str(vars->unique_id, vars->unique_id_str);
		ft_strcat(vars->tmp_filename, vars->unique_id_str);
		vars->fd = open(vars->tmp_filename, O_CREAT | O_EXCL | O_RDWR, 0600);
		if (vars->fd != -1)
			return (0);
		if (errno != EEXIST)
		{
			perror("open");
			return (-1);
		}
		vars->unique_id++;
	}
	write(2, "Failed to create unique heredoc file\n", 37);
	return (-1);
}

void	clean_filename(t_redirection *redirection)
{
	if (redirection->heredoc_filename)
	{
		unlink(redirection->heredoc_filename);
		free(redirection->heredoc_filename);
		redirection->heredoc_filename = NULL;
	}
}
