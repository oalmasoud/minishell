/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalmasou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 18:28:21 by oalmasou          #+#    #+#             */
/*   Updated: 2024/09/03 18:28:25 by oalmasou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int ft_printpercent(void)
{
	return (write(1, "%", 1));
}

int ft_printchar(int c)
{
	write(1, &c, 1);
	return (1);
}

int ft_format(char format, va_list args)
{
	int counter;

	counter = 0;
	if (format == 'c')
		counter += ft_printchar(va_arg(args, int));
	else if (format == 's')
		counter += ft_printstr(va_arg(args, char *));
	else if (format == 'p')
		counter += ft_pointeraddress(va_arg(args, void *));
	else if (format == 'd' || format == 'i')
		counter += ft_printnbr(va_arg(args, int));
	else if (format == 'u')
		counter += ft_unsigned(va_arg(args, unsigned int));
	else if (format == 'x' || format == 'X')
		counter += ft_printhex(va_arg(args, unsigned int), format);
	else if (format == '%')
		counter += ft_printpercent();
	return (counter);
}

int ft_printf(const char *placeholders, ...)
{
	int i;
	int counter;
	va_list args;

	if (!placeholders)
		return (0);
	i = 0;
	counter = 0;
	if (!placeholders)
		return (0);
	va_start(args, placeholders);
	while (placeholders[i] != '\0')
	{
		if (placeholders[i] == '%')
		{
			counter += ft_format(placeholders[i + 1], args);
			i++;
		}
		else
			counter += ft_printchar(placeholders[i]);
		i++;
	}
	va_end(args);
	return (counter);
}
