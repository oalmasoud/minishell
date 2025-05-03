/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalmasou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 19:26:26 by oalmasou          #+#    #+#             */
/*   Updated: 2024/08/30 19:26:29 by oalmasou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void ft_putendl_fd(char *s, int fd)
{
	size_t len;

	if (s == NULL)
	{
		return;
	}
	len = ft_strlen(s);
	if (len > 0)
	{
		write(fd, s, len);
	}
	write(fd, "\n", 1);
}
