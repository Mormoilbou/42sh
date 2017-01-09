/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_storage.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoehler <fkoehler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/18 11:04:50 by fkoehler          #+#    #+#             */
/*   Updated: 2016/12/11 17:36:05 by MrRobot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static t_btree	*new_cmd_link(void)
{
	t_btree	*new;

	if (!(new = (t_btree *)malloc(sizeof(*new))))
		ft_put_error(ER_MEM, 1);
	new->str = NULL;
	new->redir = NULL;
	new->left = NULL;
	new->right = NULL;
	return (new);
}

static int		strrchr_logical_op(char *s)
{
	int		i;
	char	c;

	i = ft_strlen(s) - 1;
	while (i > 0)
	{
		if (ft_isquote(s[i]) && !is_chr_escaped(s, i) && (c = s[i--]))
		{
			while (i > 0 && (s[i] != c || (s[i] == c && is_chr_escaped(s, i))))
				i--;
		}
		else if ((s[i] == '|' || s[i] == '&') &&
				s[i - 1] == s[i] && !is_chr_escaped(s, i - 1))
			return (i);
		if (i > 0)
			i--;
	}
	return (-1);
}

static void		set_str_link(t_shell *shell, t_btree **link, char *str)
{
	char	*cmd;

	(*link)->type = CMD;
	cmd = ft_back_q(shell, char_to_input(str), NULL);
	free(str);
	ft_cursh(&cmd);
	ft_glob(&cmd);
	(*link)->str = cmd;
}

t_btree			*store_cmd(char *str)
{
	int		i;
	t_shell	*shell;
	t_btree	*new;

	shell = get_struct(0);
	new = new_cmd_link();
	if ((i = strrchr_outside_quotes(str, ';')) != -1)
		new->type = SEM;
	else if ((i = strrchr_logical_op(str)) != -1)
		new->type = (str[i] == '&') ? AND : OR;
	else if ((i = strrchr_outside_quotes(str, '|')) != -1)
		new->type = PIP;
	if (i != -1 && ++i)
	{
		if (new->type == AND || new->type == OR)
			new->left = store_cmd(ft_strsub(str, 0, i - 2));
		else
			new->left = store_cmd(ft_strsub(str, 0, i - 1));
		new->right = store_cmd(ft_strsub(str, i, (ft_strlen(str) - i)));
		free(str);
	}
	else
		set_str_link(shell, &new, str);
	return (new);
}
