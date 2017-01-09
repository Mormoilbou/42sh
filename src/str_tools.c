/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoehler <fkoehler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/19 16:42:14 by fkoehler          #+#    #+#             */
/*   Updated: 2016/11/30 18:38:20 by fkoehler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int		strchr_outside_quotes(char *s, char to_find)
{
	int		i;
	char	c;

	i = 0;
	while (s[i])
	{
		if (ft_isquote(s[i]) && (c = s[i++]))
		{
			while (s[i] && s[i] != c)
				i++;
		}
		else if (s[i] == to_find && !is_chr_escaped(s, i))
			return (i);
		if (s[i])
			i++;
	}
	return (-1);
}

int		strrchr_outside_quotes(char *s, char to_find)
{
	int		i;
	char	c;

	i = ft_strlen(s) - 1;
	while (i >= 0)
	{
		if (ft_isquote(s[i]) && (c = s[i--]))
		{
			while (i >= 0 && s[i] != c)
				i--;
		}
		else if (s[i] == to_find && !is_chr_escaped(s, i))
			return (i);
		if (i >= 0)
			i--;
	}
	return (-1);
}
