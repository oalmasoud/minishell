/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalmasou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 16:06:13 by oalmasou          #+#    #+#             */
/*   Updated: 2024/08/30 16:21:08 by oalmasou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*to;
	const unsigned char	*from;
	size_t				i;

	from = (const unsigned char *)src;
	to = (unsigned char *)dest;
	i = 0;
	if (to == from || n == 0)
		return (dest);
	if (to < from)
		ft_memcpy(to, from, n);
	else
	{
		i = n;
		while (i > 0)
		{
			to[i - 1] = from[i - 1];
			i--;
		}
	}
	return (dest);
}
