/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tabnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: MrRobot <mimazouz@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/09 16:42:49 by MrRobot           #+#    #+#             */
/*   Updated: 2016/06/22 15:39:13 by MrRobot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_tabnew(size_t size)
{
	char	**tab;
	size_t	i;

	i = 0;
	tab = (char **)malloc(sizeof(char *) * (size + 1));
	while (i < size)
	{
		tab[i] = NULL;
		i++;
	}
	tab[size] = NULL;
	return (tab);
}
