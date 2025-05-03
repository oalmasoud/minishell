/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalmasou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 18:34:02 by oalmasou          #+#    #+#             */
/*   Updated: 2024/08/30 18:34:42 by oalmasou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	const char	*a;
	const char	*b;
	size_t		temp_len;

	if (*needle == '\0')
		return ((char *)haystack);
	while (*haystack != '\0' && len > 0)
	{
		a = haystack;
		b = needle;
		temp_len = len;
		while (*b != '\0' && *b == *a && temp_len > 0)
		{
			b++;
			a++;
			temp_len--;
		}
		if (*b == '\0')
		{
			return ((char *)haystack);
		}
		haystack++;
		len--;
	}
	return (NULL);
}
