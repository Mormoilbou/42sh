/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cindex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hponcet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/18 18:33:26 by hponcet           #+#    #+#             */
/*   Updated: 2016/11/13 17:25:41 by hponcet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_cindex(char *str, char c)
{
	int		i;

	i = 0;
	while (str[i] != c && str[i] != '\0')
		i++;
	if (str[i] != c)
		return (-1);
	return (i);
}

int		ft_cindex_rev(char *str, char c)
{
	int		len;

	if (!str)
		return (-1);
	len = ft_strlen(str) - 1;
	while (len > -1 && str[len] != c)
		len--;
	return (len);
}
