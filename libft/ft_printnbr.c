/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printnbr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalmasou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 18:36:50 by oalmasou          #+#    #+#             */
/*   Updated: 2024/09/03 18:36:52 by oalmasou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int ft_printnbr(int nbr)
{
	int counter;
	char *num;

	num = ft_itoa(nbr);
	counter = ft_printstr(num);
	return (counter);
}
