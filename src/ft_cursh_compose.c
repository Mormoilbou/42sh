/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cursh_compose.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hponcet <hponcet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/28 16:34:00 by hponcet           #+#    #+#             */
/*   Updated: 2016/11/29 15:44:12 by hponcet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void		ft_cursh_compose_recurs(char **str, char **tmp, char **last_str)
{
	int		i;
	char	*dup;

	i = -1;
	while (tmp[++i])
	{
		dup = ft_joinf("%s%s", tmp[i], last_str[0]);
		ft_cursh_compose(&dup);
		free(tmp[i]);
		tmp[i] = dup;
	}
	ft_strdel(last_str);
	free(str[0]);
	str[0] = ft_strnew(0);
	i = 0;
	while (tmp[i])
		str[0] = ft_joinf("%xs %xs", str[0], tmp[i++]);
	free(tmp);
}

void			ft_cursh_compose(char **str)
{
	char	**tmp;
	char	*last_str;
	int		i;

	ft_cursh_replace(str);
	tmp = NULL;
	tmp = ft_strsplit(str[0], ' ');
	i = 0;
	while (tmp[i])
		i++;
	last_str = ft_strdup(tmp[i - 1]);
	if (i >= 1 && ft_cursh_check(tmp[i - 1]))
		ft_strdel(&tmp[i - 1]);
	else
	{
		free_tab(tmp);
		ft_strdel(&last_str);
		return ;
	}
	ft_cursh_compose_recurs(str, tmp, &last_str);
}

void			ft_cursh_compose_end(char **str)
{
	int		ecursh;
	char	*start;
	char	*end;
	char	*ret;

	ecursh = ft_cindex_rev(str[0], '}') + 1;
	start = ft_strsub(str[0], 0, ecursh);
	end = ft_strsub(str[0], ecursh, ft_strlen(str[0]) - ecursh);
	ret = ft_joinf("%s{%s,}", start, end);
	ft_strdel(&end);
	ft_strdel(&start);
	free(str[0]);
	str[0] = ret;
}
