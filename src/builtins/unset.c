/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoehler <fkoehler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/30 16:04:43 by fkoehler          #+#    #+#             */
/*   Updated: 2016/11/30 16:42:28 by fkoehler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int		ft_unset(char **cmd, t_env **env_lst)
{
	int	i;

	i = 1;
	if (!cmd[i])
	{
		env_var_error(0, cmd[0], "");
		return (1);
	}
	while (cmd[i])
	{
		if (check_env_var(cmd[i], cmd[0]) == -1)
			return (1);
		del_env_var(env_lst, cmd[i], 1);
		i++;
	}
	return (0);
}
