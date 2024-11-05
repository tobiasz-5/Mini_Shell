/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer8_gen_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: negambar <negambar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 22:46:25 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/05 16:16:57 by negambar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniheader.h"

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '$');
}

static int	get_num_len(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n)
	{
		len++;
		n /= 10;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*result;
	long	nbr;
	int		len;

	nbr = n;
	len = get_num_len(n);
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	result[len] = '\0';
	if (nbr == 0)
		result[0] = '0';
	if (nbr < 0)
	{
		result[0] = '-';
		nbr = -nbr;
	}
	while (nbr)
	{
		result[--len] = nbr % 10 + '0';
		nbr /= 10;
	}
	return (result);
}
