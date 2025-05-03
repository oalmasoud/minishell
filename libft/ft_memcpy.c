/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalmasou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 15:49:59 by oalmasou          #+#    #+#             */
/*   Updated: 2024/08/30 15:50:45 by oalmasou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*to;
	const unsigned char	*from;

	if (!dest && !src)
	{
		return (NULL);
	}
	i = 0;
	to = (unsigned char *)dest;
	from = (const unsigned char *)src;
	while (n > 0)
	{
		to[i] = from[i];
		i++;
		n--;
	}
	return (dest);
}
