/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strsplit_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoehler <fkoehler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/02 11:44:16 by fkoehler          #+#    #+#             */
/*   Updated: 2016/12/11 17:31:23 by fkoehler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int	cntwords(char const *s, int i, int n, char c)
{
	if (!s)
		return (0);
	while (s[i])
	{
		if (ft_isquote(s[i]) && (i == 0 || s[i - 1] != '\\'))
		{
			c = s[i++];
			while (s[i] && (s[i] != c || (s[i] == c && s[i - 1] == '\\')))
				i++;
			!s[i] || !s[i + 1] || s[i + 1] == ' ' || s[i + 1] == '\t' ? n++ : 0;
		}
		else if ((s[i] != ' ' || (s[i] == ' ' && i > 0 && s[i - 1] == '\\'))
				&& s[i] != '\t')
		{
			while (s[i] && (s[i] != ' ' || (s[i] == ' ' && i > 0
					&& s[i - 1] == '\\')) && s[i] != '\t' && (!ft_isquote(s[i])
					|| (ft_isquote(s[i]) && s[i - 1] == '\\')))
				i++;
			s[i] && ft_isquote(s[i]) ? i-- : n++;
		}
		s[i] != 0 ? ++i : 0;
	}
	return (n);
}

static int	word_len(char const *s, int i)
{
	char	c;

	c = 0;
	if (!s[i])
		return (i);
	if (ft_isquote(s[i]) && (i == 0 || s[i - 1] != '\\'))
	{
		c = s[i++];
		while (s[i] && (s[i] != c || (s[i] == c && s[i - 1] == '\\')))
			i++;
		if (s[i] && s[++i] && s[i] != ' ' && s[i] != '\t')
			i = word_len(s, i);
	}
	else
	{
		while (s[i] && (s[i] != ' ' || (s[i] == ' ' && i > 0
				&& s[i - 1] == '\\')) && s[i] != '\t' &&
				(!ft_isquote(s[i]) || (ft_isquote(s[i]) && s[i - 1] == '\\')))
			i++;
		if (s[i] && ft_isquote(s[i]) && s[i + 1])
			i = word_len(s, i);
		else if (s[i] && ft_isquote(s[i]))
			i++;
	}
	return (i);
}

char		**strsplit_args(char const *s)
{
	int		i;
	int		j;
	int		start;
	char	**array;

	i = 0;
	j = 0;
	if (!s || !s[0])
		return (NULL);
	if (!(array = (char **)malloc(sizeof(char *) * (cntwords(s, 0, 0, 0) + 1))))
		ft_put_error(ER_MEM, 1);
	while (s[i])
	{
		while (s[i] == ' ' || s[i] == '\t')
			i++;
		start = i;
		i = word_len(s, start);
		if (start != i)
			array[j++] = ft_strsub(s, start, (i - start));
	}
	array[j] = NULL;
	return (array);
}
