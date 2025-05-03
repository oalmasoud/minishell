/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unsigned.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalmasou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 18:39:07 by oalmasou          #+#    #+#             */
/*   Updated: 2024/09/03 18:39:09 by oalmasou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int ft_unsigned(unsigned int num)
{
	char buffer[11];
	int i;

	i = 10;
	buffer[i] = '\0';
	if (num == 0)
	{
		buffer[--i] = '0';
	}
	while (num != 0)
	{
		buffer[--i] = (num % 10) + '0';
		num /= 10;
	}
	return (write(1, &buffer[i], 10 - i));
}