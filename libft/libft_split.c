/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 10:51:42 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/28 10:51:44 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

static char	**free_all_split(char **matrice, int i)
{
	while (--i >= 0)
	{
		free(matrice[i]);
		matrice[i] = NULL;
	}
	free(matrice);
	matrice = NULL;
	return (NULL);
}

static char	**ft_matrice(char const *s, char **matrice, int n_token, char c)
{
	int	i;
	int	j;
	int	len;
	int	back;

	i = 0;
	j = 0;
	while (i < n_token)
	{
		while (s[j] == c)
			j++;
		len = 0;
		while (s[j + len] != c && s[j + len] != '\0')
			len++;
		matrice[i] = (char *)malloc((len + 1) * sizeof(char));
		if (!matrice[i])
			return (free_all_split(matrice, i));
		matrice[i][len] = '\0';
		back = len;
		while (len-- > 0)
			matrice[i][len] = s[j + len];
		j = j + back;
		i++;
	}
	return (matrice);
}

static int	ft_find_token(const char *s, char c)
{
	int	flag;
	int	token;

	flag = 0;
	token = 0;
	while (*s)
	{
		if (*s == c)
			flag = 0;
		else if (flag == 0)
		{
			token++;
			flag = 1;
		}
		s++;
	}
	return (token);
}

char	**ft_split(char const *s, char c)
{
	int		n_token;
	char	**matrice;

	if (s == NULL || *s == '\0')
	{
		matrice = ft_calloc(1, sizeof(char *));
		return (matrice);
	}
	n_token = ft_find_token(s, c);
	matrice = ft_calloc((n_token + 1), sizeof(char *));
	if (!matrice)
		return (NULL);
	matrice[n_token] = NULL;
	return (ft_matrice(s, matrice, n_token, c));
}
