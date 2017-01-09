/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unsetenv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoehler <fkoehler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/26 20:56:41 by fkoehler          #+#    #+#             */
/*   Updated: 2016/12/11 17:37:43 by fkoehler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int		del_env_var(t_env **env_lst, char *var, int local)
{
	t_env	*tmp1;
	t_env	*tmp2;

	if (!(tmp1 = *env_lst))
		return (-1);
	if (!(ft_strcmp(tmp1->var, var)) && (!tmp1->local || local))
	{
		*env_lst = tmp1->next;
		free_env_var(tmp1);
		return (0);
	}
	while (tmp1->next && ft_strcmp(tmp1->next->var, var) != 0)
		tmp1 = tmp1->next;
	if (tmp1->next && (!tmp1->next->local || local))
	{
		tmp2 = tmp1->next->next;
		free_env_var(tmp1->next);
		tmp1->next = tmp2;
	}
	return (0);
}

int		ft_unsetenv(char **cmd, t_env **env_lst)
{
	int		i;

	i = 1;
	if (!cmd[1])
	{
		env_var_error(0, cmd[0], "");
		return (1);
	}
	while (cmd[i])
	{
		if (check_env_var(cmd[i], cmd[0]) == -1)
			return (1);
		del_env_var(env_lst, cmd[i], 0);
		i++;
	}
	return (0);
}
