/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_char.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoehler <fkoehler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/11 18:49:33 by fkoehler          #+#    #+#             */
/*   Updated: 2016/12/11 17:24:45 by fkoehler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int	replace_process_value(char **s, int i, int quote)
{
	int		j;
	t_shell	*shell;
	char	*tmp1;
	char	*tmp2;

	if (quote == 1)
		return (1);
	shell = get_struct(0);
	if ((*s)[i + 1] == '?')
		tmp1 = ft_itoa(shell->status);
	else if ((*s)[i + 1] == '$')
		tmp1 = ft_itoa(shell->pid);
	j = ft_strlen(tmp1);
	tmp2 = ft_replace_str(*s, i, 2, tmp1);
	ft_strdel(&tmp1);
	free(*s);
	*s = tmp2;
	return (j);
}

int	replace_backslash(char **s, int i, int quote)
{
	char	*tmp;

	if (quote == 1 && (!(*s)[i + 1] ||
		((*s)[i + 1] != 'n' && (*s)[i + 1] != '\\')))
		return (1);
	tmp = ft_replace_str(*s, i, 1, "");
	free(*s);
	*s = tmp;
	if ((*s)[i] == '\\' || (*s)[i] == '$' || (*s)[i] == 'n')
	{
		if ((*s)[i] == 'n')
			(*s)[i] = '\n';
		return (1);
	}
	return (0);
}

int	replace_tilde(char **s, int i, int quote)
{
	char	*tmp;
	t_env	*home;
	t_shell *shell;

	if (quote == 1)
		return (1);
	shell = get_struct(0);
	home = get_env_ptr(shell->env_lst, "HOME");
	if (!home || !home->val[0])
		return (1);
	tmp = ft_replace_str(*s, i, 1, home->val);
	free(*s);
	*s = tmp;
	return (ft_strlen(home->val));
}
