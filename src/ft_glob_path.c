/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_glob_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hponcet <hponcet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/19 22:45:24 by hponcet           #+#    #+#             */
/*   Updated: 2016/12/05 16:48:07 by hponcet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void	getroot(char **ret)
{
	int		len;
	char	*tmp;

	if (ret[1][1])
	{
		len = ft_strlen(ret[1]) - 1;
		tmp = ft_strsub(ret[1], 1, len);
		free(ret[1]);
		ret[1] = tmp;
		ret[0] = ft_strdup("/");
	}
	else
	{
		ft_strdel(&ret[1]);
		ret[0] = ft_strdup("/");
	}
}

static void	getpathroot(char **ret)
{
	int		i;
	char	*tmp;

	i = ft_cindex_rev(ret[1], '/');
	if (ret[1][i + 1])
	{
		tmp = ft_strsub(ret[1], i + 1, ft_strlen(ret[1]) - i);
		ret[0] = ft_strsub(ret[1], 0, i + 1);
		free(ret[1]);
		ret[1] = tmp;
	}
	else
	{
		ret[0] = ft_strdup(ret[1]);
		free(ret[1]);
		ret[1] = ft_strnew(0);
	}
}

static void	getpath(char **ret)
{
	int		i;
	char	*path;
	char	*find;

	i = ft_cindex_rev(ret[1], '/');
	find = ft_strsub(ret[1], i + 1, ft_strlen(ret[1]) - i);
	path = ft_strsub(ret[1], 0, i + 1);
	ret[0] = ft_strdup(path);
	ft_strdel(&path);
	free(ret[1]);
	ret[1] = find;
}

static void	getpathhome(char **ret)
{
	int		i;
	char	*path;
	char	*find;
	char	*home;

	i = ft_cindex_rev(ret[1], '/');
	find = ft_strsub(ret[1], i + 1, ft_strlen(ret[1]) - i);
	path = ft_strsub(ret[1], 1, i);
	home = env_var_to_value("HOME");
	ret[0] = ft_strjoin(home, path);
	ft_strdel(&path);
	ft_strdel(&home);
	free(ret[1]);
	ret[1] = find;
}

void		ft_glob_path(char **ret)
{
	char	*pwd;
	int		i;

	pwd = NULL;
	pwd = getcwd(pwd, MAXPATHLEN);
	i = ft_cindex_rev(ret[1], '/');
	if (i == 0)
		getroot(ret);
	else if (ret[1][0] == '/' && i > 0)
		getpathroot(ret);
	else if (ret[1][0] == '~' && i > 0)
		getpathhome(ret);
	else if (ret[1][0] != '/' && i > 0)
		getpath(ret);
	if (ret[0] && ret[0][0] == '/')
		ret[2] = ft_strdup(ret[0]);
	else if (ret[0] && ret[0][0] != '/')
		ret[2] = ft_joinf("%s/%s", pwd, ret[0]);
	else
	{
		ret[0] = ft_strjoin(pwd, "/");
		ret[2] = ft_strjoin(pwd, "/");
	}
	ft_strdel(&pwd);
}

/*
** ret[0] = path
** ret[1] = name
** ret[2] = absolute path
*/
