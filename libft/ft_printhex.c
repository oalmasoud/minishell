/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printhex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalmasou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 18:36:11 by oalmasou          #+#    #+#             */
/*   Updated: 2024/09/03 18:36:13 by oalmasou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int ft_printhex(unsigned int nbr, char format)
{
	const char *buffer = "0123456789abcdef";
	char output[9];
	char c;
	int i;
	int counter;

	i = 0;
	counter = 0;
	if (nbr == 0)
		output[i++] = '0';
	while (nbr > 0)
	{
		output[i++] = buffer[nbr % 16];
		nbr /= 16;
	}
	while (i > 0)
	{
		c = output[--i];
		if (format == 'X')
			c = ft_toupper(c);
		counter += write(1, &c, 1);
	}
	return (counter);
}
