/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 14:19:12 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/26 14:19:15 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

char	*check_absolute_or_relative(const char *cmd_name)
{
	if (cmd_name[0] == '/' || cmd_name[0] == '.')
	{
		if (access(cmd_name, X_OK) == 0)
			return (ft_strdup(cmd_name));
		return (NULL);
	}
	return (NULL);
}

char	**get_path_directories(t_shell_state *shell_state)
{
	char		*path_env;
	t_env_var	*path_var;
	char		**paths;

	path_var = find_env_var(shell_state->env_list, "PATH");
	if (path_var)
		path_env = path_var->value;
	else
		path_env = NULL;
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	return (paths);
}

char	*build_full_path(const char *path, const char *cmd_name)
{
	size_t	path_len;
	size_t	cmd_len;
	char	*full_path;

	path_len = ft_strlen(path);
	cmd_len = ft_strlen(cmd_name);
	full_path = malloc(path_len + cmd_len + 2);
	if (!full_path)
		return (NULL);
	ft_strcpy(full_path, path);
	ft_strcat(full_path, "/");
	ft_strcat(full_path, cmd_name);
	return (full_path);
}

char	*search_in_paths(const char *cmd_name, char **paths)
{
	struct stat	path_stat;
	char		*full_path;
	int			i;

	i = 0;
	while (paths[i] != NULL)
	{
		full_path = build_full_path(paths[i], cmd_name);
		if (!full_path)
			return (NULL);
		stat(full_path, &path_stat);
		if (access(full_path, X_OK) == 0 && S_ISREG(path_stat.st_mode))
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*get_command_path(const char *cmd_name, t_shell_state *shell_state)
{
	char	*command_path;
	char	**paths;

	command_path = check_absolute_or_relative(cmd_name);
	if (command_path)
		return (command_path);
	paths = get_path_directories(shell_state);
	if (!paths)
		return (NULL);
	command_path = search_in_paths(cmd_name, paths);
	free_args_array(paths);
	return (command_path);
}
