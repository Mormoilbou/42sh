/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_glob.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hponcet <hponcet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/22 17:19:23 by hponcet           #+#    #+#             */
/*   Updated: 2016/12/13 13:14:48 by hponcet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int	ft_glob_count_quote_rev(char *s, int i, char c)
{
	int		j;

	j = 1;
	i--;
	while (i >= 0 && s[i] && s[i] != c)
	{
		i--;
		j++;
	}
	return (j);
}

int			ft_glob_check(char *str)
{
	int		len;

	if (!str)
		return (0);
	len = ft_strlen(str) - 1;
	while (len >= 0)
	{
		if (len > 0 && (str[len - 1] == '\\'
					|| (str[len] == '?' && str[len - 1] == '$')))
		{
			len -= 2;
			continue ;
		}
		if (str[len] == '[' || str[len] == '?' || str[len] == '*')
			return (1);
		if (str[len] == '\"' || str[len] == '\'' || str[len] == 96)
			len -= ft_glob_count_quote_rev(str, len, str[len]);
		len--;
	}
	return (0);
}

int			ft_glob_nomatch(char *nm, char *ret, char **tabl, char **cmd)
{
	t_shell *shell;

	shell = get_struct(0);
	shell->status = 1;
	ft_putstr_fd("42sh: no matches found: ", STDERR_FILENO);
	ft_putendl_fd(nm, STDERR_FILENO);
	free_tab(cmd);
	ft_strdel(&nm);
	ft_strdel(&ret);
	free(tabl[0]);
	tabl[0] = ft_strdup(" ");
	return (1);
}

static char	*ft_glob_join(char **cmd, char **tabl)
{
	char	*find;
	char	*tmp;
	char	*ret;
	int		i;

	i = -1;
	ret = ft_strnew(0);
	tmp = NULL;
	while (cmd[++i])
	{
		find = ft_strdup(cmd[i]);
		if (ft_glob_check(cmd[i]) == 1 && (tmp = ft_glob_replace(cmd[i]))
				&& tmp[0])
			ret = ft_joinf("%xs %s", ret, tmp);
		else if (ft_glob_check(cmd[i]) == 1 && (!tmp || !tmp[0]))
		{
			ft_glob_nomatch(find, ret, tabl, cmd);
			return (NULL);
		}
		else
			ret = ft_joinf("%xs %s", ret, cmd[i]);
		ft_strdel(&tmp);
		ft_strdel(&find);
	}
	return (ret);
}

int			ft_glob(char **tabl)
{
	char	**cmd;
	char	*ret;

	if (!tabl[0] || !tabl[0][0])
		return (1);
	cmd = ft_strsplit(tabl[0], ' ');
	if (!(ret = ft_glob_join(cmd, tabl)))
		return (1);
	free_tab(cmd);
	free(tabl[0]);
	tabl[0] = NULL;
	if (ret)
		tabl[0] = ft_strdup(ret + 1);
	ft_strdel(&ret);
	return (0);
}
