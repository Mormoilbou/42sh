/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_glob_sortchain.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hponcet <hponcet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/25 16:44:42 by hponcet           #+#    #+#             */
/*   Updated: 2016/11/22 17:29:40 by hponcet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void	ft_glob_sortchain(t_glob **ret, t_glob *file)
{
	t_glob	*tmp;

	tmp = *ret;
	if (!*ret)
	{
		*ret = file;
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = file;
}
