/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hponcet <hponcet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/13 15:36:51 by hponcet           #+#    #+#             */
/*   Updated: 2016/12/12 13:18:28 by hponcet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int			compl_wis(char *str)
{
	int		i;
	int		j;

	if (!str)
		return (0);
	i = ft_strlen(str) - 1;
	j = 0;
	while (str[j] && (str[j] == ' ' || str[j] == '	'))
		j++;
	if (i == j)
		return (0);
	while (i >= j)
	{
		if (str[i] == ' ' && i > 0 && str[i - 1] != ' ' && str[i - 1] != ';'
				&& str[i - 1] != '`')
			return (1);
		if (str[i] == ';' || str[i] == '`')
			return (0);
		i--;
	}
	return (0);
}

void		compl_delchain(t_compl *chain)
{
	t_compl	*tmp;

	chain = chain->prev;
	while (chain && chain->id != 1)
	{
		tmp = chain;
		chain->type = 0;
		chain->len = 0;
		chain->id = 0;
		ft_strdel(&chain->name);
		chain->next = NULL;
		chain = chain->prev;
		free(tmp);
	}
	if (!chain)
		return ;
	chain->type = 0;
	chain->len = 0;
	chain->id = 0;
	ft_strdel(&(chain->name));
	chain->next = NULL;
	free(chain);
}

void		compl_file(t_shell *shell, char *str)
{
	char	**ret;
	t_compl	*dir;

	dir = NULL;
	ret = (char**)malloc(sizeof(char*) * 3);
	ret[2] = NULL;
	ret[1] = compl_getfind(str);
	if (compl_getpath(shell, ret) == 1)
	{
		ft_strdel(&ret[1]);
		free(ret);
		return ;
	}
	dir = compl_makechain(ret[0], dir, ret[1]);
	if (!ret[1])
		ret[1] = ft_strdup("");
	if (dir)
		compl_display(shell, dir, ret[1]);
	ft_strdel(&ret[0]);
	ft_strdel(&ret[1]);
	free(ret);
}

void		compl_bin(t_shell *shell, char *str)
{
	char	**path;
	char	*find;
	t_compl	*dir;
	int		i;

	dir = NULL;
	path = compl_pathbin(shell);
	if (!path)
		return ;
	find = ft_strdup(str);
	i = 0;
	while (path && path[i])
	{
		dir = compl_makechain(path[i], dir, find);
		i++;
	}
	free_tab(path);
	if (dir)
		compl_display(shell, dir, find);
	ft_strdel(&find);
}

void		compl(t_shell *shell)
{
	char	*str;

	if (!shell->input)
		return ;
	str = input_to_char(shell->input);
	if (compl_wis(str) == 1)
		compl_file(shell, str);
	else
		compl_bin(shell, str);
	ft_strdel(&str);
}
