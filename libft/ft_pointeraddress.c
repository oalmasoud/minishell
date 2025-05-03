/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pointeraddress.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalmasou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 18:18:53 by oalmasou          #+#    #+#             */
/*   Updated: 2024/09/03 18:19:08 by oalmasou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int ft_put_ptr(unsigned long long ptr)
{
	char hex[17] = "0123456789abcdef";
	char output[20];
	int i;
	int count;

	i = 0, count = 0;
	if (ptr == 0)
		output[i++] = '0';
	else
	{
		while (ptr)
		{
			output[i++] = hex[ptr % 16];
			ptr /= 16;
		}
	}
	while (i--)
	{
		write(1, &output[i], 1);
		count++;
	}
	return (count);
}

int ft_pointeraddress(void *ptr)
{
	int counter;
	unsigned long long uptr;

	counter = 0;
	uptr = (unsigned long long)ptr;
	if (uptr == 0)
		counter += write(1, "(nil)", 5);
	else
	{
		counter += write(1, "0x", 2);
		counter += ft_put_ptr(uptr);
	}
	return (counter);
}
