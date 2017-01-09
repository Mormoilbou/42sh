/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_tools_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoehler <fkoehler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/29 20:04:05 by fkoehler          #+#    #+#             */
/*   Updated: 2016/12/07 16:30:34 by fkoehler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

t_env	*get_last_env_elem(t_env *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

size_t	input_part_len(t_input *start, t_input *end)
{
	int	i;

	i = 0;
	while (start && ++i && start != end)
		start = start->next;
	return (i);
}
