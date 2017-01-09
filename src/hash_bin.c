/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_bin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hponcet <hponcet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/17 21:45:34 by hponcet           #+#    #+#             */
/*   Updated: 2016/12/14 16:46:39 by hponcet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

t_hash		**hash_bin(t_shell *shell)
{
	t_hash	**htbl;
	char	**paths;
	int		i;

	i = 0;
	htbl = hash_newtbl(HASHLEN);
	if (!(paths = compl_pathbin(shell)))
		return (htbl);
	while (paths[i])
	{
		hash_addpath(htbl, paths[i], HASHLEN);
		i++;
	}
	free_tab(paths);
	return (htbl);
}
