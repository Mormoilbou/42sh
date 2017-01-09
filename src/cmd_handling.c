/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_handling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoehler <fkoehler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/15 14:41:46 by fkoehler          #+#    #+#             */
/*   Updated: 2016/12/11 15:06:17 by MrRobot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int		binary_cmd(char **cmd, char **env_array, t_env *env_lst, t_hash **htbl)
{
	int		abs_path;
	char	*bin_path;
	t_env	*home;

	abs_path = 0;
	bin_path = NULL;
	if (ft_strchr(cmd[0], '/') != NULL && cmd[0][0] == '~' && ++abs_path)
	{
		if (!(home = get_env_ptr(env_lst, "HOME")) || !home->val[0])
			return (cd_error(0, ""));
		bin_path = ft_strjoin(home->val, cmd[0] + 1);
	}
	else if (ft_strchr(cmd[0], '/') != NULL && ++abs_path)
		bin_path = ft_strdup(cmd[0]);
	else if ((bin_path = hash_search(htbl, cmd[0], HASHLEN)))
		;
	else if (!(bin_path = get_bin_path(cmd[0], env_lst)))
		return (-1);
	if (abs_path && ((abs_path = check_bin_path(bin_path, cmd[0])) != 0))
	{
		free(bin_path);
		return (abs_path == 1) ? exec_error(1, cmd[0]) : (-1);
	}
	return (exec_bin(bin_path, cmd, env_array));
}

int		builtins_cmd(char **cmd, t_env *env_lst, t_shell *shell)
{
	if (ft_strcmp(cmd[0], "cd") == 0)
		shell->status = ft_cd(cmd, env_lst);
	if (ft_strcmp(cmd[0], "history") == 0)
		shell->status = ft_history(cmd, shell);
	else if (ft_strcmp(cmd[0], "echo") == 0)
		shell->status = ft_echo(cmd, shell->fd[1]);
	else if (ft_strcmp(cmd[0], "env") == 0)
		shell->status = ft_env(cmd, env_lst, 1, shell);
	else if (ft_strcmp(cmd[0], "setenv") == 0)
		shell->status = ft_setenv(++cmd, &env_lst, 0);
	else if (ft_strcmp(cmd[0], "unsetenv") == 0)
		shell->status = ft_unsetenv(cmd, &env_lst);
	else if (ft_strcmp(cmd[0], "exit") == 0)
		shell->status = ft_exit(cmd, shell);
	else if (ft_strcmp(cmd[0], "read") == 0)
		shell->status = ft_read(cmd, &env_lst);
	else if (ft_strcmp(cmd[0], "export") == 0)
		shell->status = ft_export(cmd, env_lst);
	else if (ft_strcmp(cmd[0], "unset") == 0)
		shell->status = ft_unset(cmd, &env_lst);
	else
		return (-1);
	return (0);
}

int		handle_cmd(t_shell *shell, t_btree *link, int already_forked)
{
	char	**cmd;
	char	**env_array;

	env_array = NULL;
	if (!(cmd = parse_cmd(shell->env_lst, link)))
		return (-1);
	if (link->redir)
		handle_redirs(shell, link, cmd, already_forked);
	else if (is_builtin(cmd[0]))
		builtins_cmd(cmd, shell->env_lst, shell);
	else
	{
		env_array = env_lst_to_array(shell->env_lst);
		if (already_forked)
			binary_cmd(cmd, env_array, shell->env_lst, shell->hash_bin);
		else
			exec_fork(cmd, env_array, shell->env_lst, shell);
		free_tab(env_array);
	}
	free_tab(cmd);
	return (0);
}

int		handle_btree(t_shell *shell, t_btree *link)
{
	if (link->type == SEM || link->type == AND || link->type == OR)
	{
		handle_btree(shell, link->left);
		if (link->type == SEM || (link->type == AND && shell->status == 0)
		|| (link->type == OR && shell->status != 0))
			handle_btree(shell, link->right);
	}
	else if (link->type == PIP)
	{
		if (!link->left || !link->right)
			return (cmd_error(0, '|', NULL));
		pipe_fork_father(shell, link);
	}
	else if (link->str && link->str[0])
		handle_cmd(shell, link, 0);
	return (0);
}
