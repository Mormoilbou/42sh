/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compl_getpath2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hponcet <hponcet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/13 13:29:56 by hponcet           #+#    #+#             */
/*   Updated: 2016/12/13 13:50:05 by hponcet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int			compl_getnorm(char **ret, char **pwd)
{
	ret[1] = ft_strnew(0);
	ret[0] = ft_strdup(pwd[0]);
	ft_strdel(pwd);
	return (0);
}

char		*ft_putain_de_norme_de_merde(char **cmd, int i)
{
	free_tabnb(cmd, i);
	return (NULL);
}

char		*ft_glob_makestr_return(t_glob *ret)
{
	char			*str;

	str = ft_glob_tglobtostr(ret);
	ft_glob_delchain(ret);
	return (str);
}
