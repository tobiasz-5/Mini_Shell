/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: negambar <negambar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:32:34 by negambar          #+#    #+#             */
/*   Updated: 2024/10/23 18:06:00 by negambar         ###   ########.fr       */
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
	while (s[i] == ' ' || s[i] == '\t')
		i++;
	while (s[i])
	{
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
	while (s[i])
	{
		printf("%c\n", s[i]);
		if (s[i] == ' ')
			i++;
		if (s[i+1] == '<' && s[i+2] == '<')
		{
			i++;
			break ;
		}
		if ((s[i] == '|' || s[i + 1] == '|'))
			break ;
		i++;
	}
	cpy = ft_strndup(s, i);
	return (cpy);
}

int	readloop(char *delimiter)
{
	char	*line;

	printf("%s\n", delimiter);
	delimiter = pipepipe(delimiter);
	delimiter = ft_strtrim(delimiter);
	printf("%s\n", delimiter);
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