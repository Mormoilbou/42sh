/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_glob_path_globed.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hponcet <hponcet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/29 16:37:29 by hponcet           #+#    #+#             */
/*   Updated: 2016/12/13 13:09:50 by hponcet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

char	**ft_glob_make_pathfind(char *cmd)
{
	char	**pth_cmd;

	pth_cmd = (char**)malloc(sizeof(char*) * 4);
	pth_cmd[3] = NULL;
	pth_cmd[2] = NULL;
	pth_cmd[0] = NULL;
	pth_cmd[1] = ft_strdup(cmd);
	return (pth_cmd);
}

void	ft_glob_addpath(t_glob **tree, char *path)
{
	t_glob	*tmp;
	t_glob	*new;

	new = (t_glob*)malloc(sizeof(t_glob));
	new->name = NULL;
	new->path = ft_strdup(path);
	new->next = NULL;
	tmp = *tree;
	if (!(*tree))
	{
		*tree = new;
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	ft_glob_maketree(char *curpath, char **paths, t_glob **tree, int i)
{
	char			*path;
	DIR				*dirp;
	struct dirent	*s_dir;

	path = NULL;
	if (!paths[i])
	{
		ft_glob_addpath(tree, curpath);
		return ;
	}
	if ((dirp = opendir(curpath)) == NULL)
		return ;
	while ((s_dir = readdir(dirp)) != NULL)
	{
		if (s_dir->d_type == 4 && ft_strcmp(s_dir->d_name, ".") != 0
				&& ft_strcmp(s_dir->d_name, "..") != 0
				&& ft_glob_compare(paths[i], s_dir->d_name) != 0)
		{
			path = ft_joinf("%s%s/", curpath, s_dir->d_name);
			ft_glob_maketree(path, paths, tree, i + 1);
			ft_strdel(&path);
		}
	}
	closedir(dirp);
}

t_glob	*ft_glob_pathtree(char *cmd)
{
	char	**paths;
	t_glob	*tree;

	if (!cmd)
		return (NULL);
	tree = NULL;
	paths = ft_strsplit(cmd, '/');
	ft_glob_maketree("/", paths, &tree, 0);
	free_tab(paths);
	if (cmd[0] == '/' && cmd[1] == '\0')
		free(paths);
	return (tree);
}
