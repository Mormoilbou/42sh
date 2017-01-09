/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_new.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hponcet <hponcet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/17 21:20:22 by hponcet           #+#    #+#             */
/*   Updated: 2016/12/11 17:19:56 by fkoehler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

t_hash	**hash_newtbl(int nb_case)
{
	t_hash	**htbl;
	int		i;

	i = nb_case - 1;
	if (!(htbl = (t_hash**)malloc(sizeof(t_hash*) * nb_case + 1)))
		return (NULL);
	while (i >= 0)
		htbl[i--] = NULL;
	return (htbl);
}

t_hash	*hash_newfile(char *name, char *value)
{
	t_hash	*file;

	file = (t_hash*)malloc(sizeof(t_hash));
	file->name = ft_strdup(name);
	file->value = ft_strdup(value);
	file->next = NULL;
	return (file);
}
