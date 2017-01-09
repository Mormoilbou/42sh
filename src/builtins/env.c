/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoehler <fkoehler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/26 20:56:41 by fkoehler          #+#    #+#             */
/*   Updated: 2016/11/30 16:43:03 by fkoehler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int	put_env(t_env *env_lst)
{
	while (env_lst)
	{
		if (!env_lst->local && env_lst->val)
		{
			ft_putstr(env_lst->var);
			ft_putchar('=');
			if (env_lst->val[0])
				ft_putendl(env_lst->val);
			else
				ft_putchar('\n');
		}
		env_lst = env_lst->next;
	}
	return (0);
}

static int	parse_env_flags(char **cmd, t_env **env_lst)
{
	if (!((*cmd)[1]))
		return (env_error(2, '\0'));
	if ((*cmd)[1] == 'i')
	{
		if ((*cmd)[2])
			return (env_error(0, (*cmd)[2]));
		free_env_lst(env_lst);
	}
	else if ((*cmd)[1] == 'u' && (*cmd)[2])
	{
		del_env_var(env_lst, (*cmd + 2), 0);
		return (1);
	}
	else if ((*cmd)[1] == 'u' && *(cmd + 1))
	{
		del_env_var(env_lst, (*(cmd + 1)), 0);
		return (2);
	}
	else if ((*cmd)[1] == 'u' && !(*(cmd + 1)))
		return (env_error(1, 'u'));
	else
		return (env_error(0, (*cmd)[1]));
	return (1);
}

static int	exec_cmd(char **cmd, t_env *env_lst_cpy, t_shell *shell)
{
	char	**env_array;

	env_array = NULL;
	if (!(ft_strcmp(cmd[0], "setenv")) || !(ft_strcmp(cmd[0], "unsetenv")))
		env_var_error(2, "env", cmd[0]);
	else if (is_builtin(cmd[0]))
		builtins_cmd(cmd, env_lst_cpy, shell);
	else
	{
		env_array = env_lst_to_array(env_lst_cpy);
		exec_fork(cmd, env_array, env_lst_cpy, shell);
		free_tab(env_array);
	}
	free_env_lst(&env_lst_cpy);
	return (0);
}

int			ft_env(char **cmd, t_env *env_lst, int i, t_shell *shell)
{
	int		j;
	t_env	*env_lst_cpy;

	env_lst_cpy = NULL;
	dup_env_lst(env_lst, &(env_lst_cpy));
	while (cmd[i])
	{
		j = 0;
		if (cmd[i][0] == '-')
		{
			if ((j = parse_env_flags(cmd + i, &env_lst_cpy)) == -1)
			{
				free_env_lst(&env_lst_cpy);
				return (1);
			}
		}
		else if (ft_strchr(cmd[i], '=') != NULL && ++j)
			ft_setenv(&cmd[i], &env_lst_cpy, 1);
		else
			return (exec_cmd(cmd + i, env_lst_cpy, shell));
		i += j;
	}
	env_lst_cpy != NULL ? put_env(env_lst_cpy) : (0);
	free_env_lst(&env_lst_cpy);
	return (0);
}
