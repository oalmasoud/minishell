/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalmasou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 18:54:05 by oalmasou          #+#    #+#             */
/*   Updated: 2024/08/30 18:54:10 by oalmasou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	int		size;
	char	*ptr;
	int		i;

	size = 0;
	i = 0;
	while (s[size] != '\0')
	{
		size++;
	}
	ptr = malloc(size + 1);
	if (ptr == NULL)
	{
		return (NULL);
	}
	while (i <= size)
	{
		ptr[i] = s[i];
		i++;
	}
	return (ptr);
}
