/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compl_str.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hponcet <hponcet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/22 16:40:47 by hponcet           #+#    #+#             */
/*   Updated: 2016/12/12 12:47:46 by hponcet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void		compl_addstrend(t_shell *shell, char *str, int type)
{
	int		i;

	i = 0;
	while (str[i])
	{
		store_input(shell, str[i]);
		ft_putchar(str[i++]);
	}
	if (type == 1)
	{
		store_input(shell, '/');
		ft_putchar('/');
	}
	else if (type == 0)
	{
		store_input(shell, ' ');
		ft_putchar(' ');
	}
}

int			compl_retstr(t_compl *tmp, t_shell *shell, int pos, int i)
{
	tputs(tgetstr("rc", 0), 1, putchar);
	tputs(tgetstr("ce", 0), 1, putchar);
	if (i == 1)
		compl_addstrend(shell, tmp->name + pos, tmp->type);
	else
		compl_addstr(shell, tmp->name + pos);
	return (1);
}

int			compl_retstrchar(t_compl *tmp, t_shell *shell, int pos, char c)
{
	char	*str;

	str = tmp->name + pos;
	tputs(tgetstr("rc", 0), 1, putchar);
	tputs(tgetstr("cd", 0), 1, putchar);
	compl_addstr(shell, str);
	store_input(shell, c);
	ft_putchar(c);
	return (1);
}

char		*compl_name_wesp(char *str)
{
	char	*ret;
	int		i;
	int		j;
	int		nb;

	i = 0;
	nb = 0;
	while (str[i])
	{
		if (str[i] == ' ' && i > 0 && str[i - 1] != '\\')
			nb++;
		i++;
	}
	ret = ft_strnew(ft_strlen(str) + nb);
	i = 0;
	j = 0;
	while (str[i])
	{
		if ((str[i] == ' ' && i > 0 && str[i - 1] != '\\')
				|| (str[i] == ' ' && i == 0))
			ret[j++] = '\\';
		ret[j++] = str[i++];
	}
	return (ret);
}

char		**compl_pathbin(t_shell *shell)
{
	t_env	*pathbin;
	char	**path;
	char	*joinpath;

	path = NULL;
	joinpath = NULL;
	if (!(pathbin = get_env_ptr(shell->env_lst, "PATH")))
	{
		ft_put_error(ERR_PATH, 0);
		return (NULL);
	}
	if ((joinpath = ft_strdup(pathbin->val)))
		path = ft_strsplit(joinpath, ':');
	ft_strdel(&joinpath);
	return (path);
}
