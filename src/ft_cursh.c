/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cursh.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hponcet <hponcet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/24 15:21:48 by hponcet           #+#    #+#             */
/*   Updated: 2016/12/09 23:32:43 by hponcet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int	ft_cursh_count_quote_rev(char *s, int i, char c)
{
	int		j;

	j = 1;
	i--;
	while (i >= 0 && s[i] != c && s[i])
	{
		i--;
		j++;
	}
	return (j);
}

int			ft_cursh_check(char *str)
{
	int		c[4];

	c[0] = 0;
	c[1] = 0;
	c[2] = 0;
	if (!str)
		return (0);
	c[3] = ft_strlen(str) - 1;
	while (c[3] >= 0)
	{
		if (c[3] > 0 && str[c[3] - 1] == '\\')
		{
			c[3] -= 2;
			continue ;
		}
		(str[c[3]] == '{') ? c[0] = 1 : (0);
		(str[c[3]] == '}') ? c[1] = 1 : (0);
		(str[c[3]] == ',') ? c[2] = 1 : (0);
		if (str[c[3]] == '\"' || str[c[3]] == '\'' || str[c[3]] == 96)
			c[3] -= ft_cursh_count_quote_rev(str, c[3], str[c[3]]);
		c[3]--;
	}
	if (c[0] > 0 && c[1] > 0 && c[2] > 0)
		return (1);
	return (0);
}

int			ft_cursh(char **tabl)
{
	int		i;
	char	**cmd;
	char	*ret;

	i = -1;
	if (!tabl[0] || !tabl[0][0])
		return (1);
	ret = ft_strnew(0);
	cmd = ft_strsplit(tabl[0], ' ');
	while (cmd[++i])
	{
		if (ft_cursh_check(cmd[i]) == 1)
			ft_cursh_compose(&cmd[i]);
		ret = ft_joinf("%xs %s", ret, cmd[i]);
	}
	free_tab(cmd);
	free(tabl[0]);
	if (ret)
		tabl[0] = ft_strdup(ret + 1);
	ft_strdel(&ret);
	return (0);
}
