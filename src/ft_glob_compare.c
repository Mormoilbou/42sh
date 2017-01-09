/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_glob_compare.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hponcet <hponcet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/23 16:04:03 by hponcet           #+#    #+#             */
/*   Updated: 2016/12/11 18:26:47 by fkoehler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int			ft_glob_compare(char *s1, char *s2)
{
	if ((!*s1 && !*s2))
		return (1);
	else if (*s1 == '?' && *s2)
		return (ft_glob_compare(s1 + 1, s2 + 1));
	else if (*s1 == '[' && s1[1] == ']')
		return (0);
	else if (*s1 == '[' && *s2)
		return (ft_glob_captain_crochet(s1 + 1, s2, s1[1]));
	else if (*s1 == '*' && s1[1] == '*')
		return (ft_glob_compare(s1 + 1, s2));
	else if (*s1 == '*' && *s2 == s1[1])
		return (ft_glob_compare(s1 + 1, s2) || ft_glob_compare(s1, s2 + 1));
	else if (*s1 == '*' && *s2 != s1[1] && *s2)
		return (ft_glob_compare(s1, s2 + 1));
	else if (*s1 == '\\' && (s1[1] == '*'
				|| s1[1] == '?' || s1[1] == '{'
				|| s1[1] == '[') && *s2 == s1[1])
		return (ft_glob_compare(s1 + 2, s2 + 1));
	else if (*s1 == *s2)
		return (ft_glob_compare(s1 + 1, s2 + 1));
	return (0);
}

int			ft_glob_captain_crochet(char *s1, char *s2, char c)
{
	if ((!*s1 && !*s2))
		return (1);
	else if (s1[1] == '-' && c <= s1[2] && s1[2] != ']')
		return (ft_glob_captain_crochet(s1 + 1, s2, s1[0]));
	else if (*s1 == '-' && s1[1] != ']' && s1[1] < c && s1[2] != ']')
		return (ft_glob_captain_crochet(s1 + 2, s2, s1[2]));
	else if (*s1 == '-' && s1[2] == ']' && s1[1] < c)
		return (0);
	else if (*s1 == '-' && s1[1] != ']' && s1[1] >= c && *s2 != c)
		return (ft_glob_captain_crochet(s1, s2, c + 1));
	else if (c != *s2 && s1[1] != ']' && s1[1] != '-')
		return (ft_glob_captain_crochet(s1 + 1, s2, s1[1]));
	else if (c == *s2 && *s1 != ']')
	{
		while (*s1 != ']')
			s1++;
		return (ft_glob_compare(s1 + 1, s2 + 1));
	}
	return (0);
}
