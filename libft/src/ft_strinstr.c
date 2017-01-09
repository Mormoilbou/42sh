/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strinstr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hponcet <hponcet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/07 14:30:06 by hponcet           #+#    #+#             */
/*   Updated: 2016/11/15 17:37:51 by hponcet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_strinstr(char *orig, char *find)
{
	int		i;
	int		j;

	if (!find)
		return (1);
	if (!find[0])
		return (1);
	i = 0;
	while (orig[i])
	{
		j = 0;
		while (orig[i + j] == find[j])
		{
			j++;
			if (!find[j])
				return (1);
		}
		i++;
	}
	return (0);
}
