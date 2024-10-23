/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: negambar <negambar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:32:34 by negambar          #+#    #+#             */
/*   Updated: 2024/10/23 18:34:40 by negambar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniheader.h"

char	*ft_strtrim(char *s)
{
	char	*cpy;
	int		i = 0;
	int		j = 0;

	cpy = (char *)malloc(strlen(s) + 1);
	if (!cpy)
		return (NULL);
	while (s[i])
	{
		if (s[i] == ' ')
			i++;
		cpy[j] = s[i];
		i++;
		j++;
	}
	cpy[j] = '\0';
	return (cpy);
}

char	*pipepipe(char *s)
{
	char	*cpy;
	int		i;

	i = 0;
	s = ft_strtrim(s);
	while (s[i])
	{
		if (s[i] == '<' && s[i + 1] == '<')
		{
			break ;
		}
		if (s[i] == '|' )
			break ;
		i++;
	}
	cpy = ft_strndup(s, i);
	return (cpy);
}

int	readloop(char *delimiter)
{
	char	*line;

	delimiter = pipepipe(delimiter);
	line = readline("> ");
	while (line && strcmp(line, delimiter) != 0)
	{
		if (!line || strcmp(line, delimiter) == 0)
		{
			break ;
		}
		line = readline("> ");
	}
	if (line)
		free(line);
	return (1);
}