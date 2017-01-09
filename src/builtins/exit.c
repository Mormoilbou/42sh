/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoehler <fkoehler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/21 20:39:30 by fkoehler          #+#    #+#             */
/*   Updated: 2016/12/11 17:38:44 by fkoehler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void	ft_exit_foot(t_shell *shell)
{
	restore_term(get_struct(0));
	hash_delhtbl(shell->hash_bin, HASHLEN);
}

int			ft_exit(char **cmd, t_shell *shell)
{
	int		i;
	int		ret;

	i = 0;
	ret = 0;
	if (!(cmd) || !cmd[1])
	{
		restore_term(get_struct(0));
		exit(EXIT_SUCCESS);
	}
	if (cmd[2])
		return (exit_error(0, ""));
	if (cmd[1][i] == '-')
		i++;
	while (cmd[1][i])
	{
		if ((!ft_isdigit(cmd[1][i++]) || i > 15) && exit_error(1, cmd[1]))
		{
			ft_exit_foot(shell);
			exit(255);
		}
	}
	ret = ft_atoi(cmd[1]);
	ft_exit_foot(shell);
	exit(ret);
}
