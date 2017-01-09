/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forking.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoehler <fkoehler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/04 18:47:45 by fkoehler          #+#    #+#             */
/*   Updated: 2016/12/14 17:41:40 by fkoehler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

pid_t	redir_fork(char **cmd, t_shell *shell)
{
	pid_t	pid;
	char	**env_array;

	env_array = env_lst_to_array(shell->env_lst);
	if ((pid = fork()) < 0)
		return ((pid_t)exec_error(0, "fork"));
	if (pid == 0)
	{
		if (dup_std_fd(shell->fd) != 0)
			exit(EXIT_FAILURE);
		if (is_builtin(cmd[0]))
			builtins_cmd(cmd, shell->env_lst, shell);
		else
			binary_cmd(cmd, env_array, shell->env_lst, shell->hash_bin);
		exit(shell->status);
	}
	else if (pid > 0)
	{
		close_and_reset_fd(shell->fd);
		waitpid(pid, &shell->status, 0);
		set_status(&shell->status, cmd);
		free_tab(env_array);
	}
	return (pid);
}

pid_t	exec_fork(char **cmd, char **env_array, t_env *env_lst, t_shell *shell)
{
	pid_t	pid;

	if ((pid = fork()) < 0)
		return ((pid_t)exec_error(0, "fork"));
	if (pid == 0)
	{
		if (dup_std_fd(shell->fd) != 0)
			exit(EXIT_FAILURE);
		binary_cmd(cmd, env_array, env_lst, shell->hash_bin);
		exit(shell->status);
	}
	else if (pid > 0)
	{
		close_and_reset_fd(shell->fd);
		waitpid(pid, &shell->status, 0);
		set_status(&shell->status, cmd);
	}
	return (pid);
}

pid_t	pipe_fork_father(t_shell *shell, t_btree *link)
{
	pid_t	pid;

	if ((pid = fork()) < 0)
		return ((pid_t)exec_error(0, "fork"));
	if (pid == 0)
	{
		pipe_fork_child(shell, link);
		exit(shell->status);
	}
	else if (pid > 0)
	{
		waitpid(pid, &shell->status, 0);
		set_status(&shell->status, &link->left->str);
	}
	return (pid);
}

pid_t	pipe_fork_child(t_shell *shell, t_btree *link)
{
	pid_t	pid;
	int		fd[2];

	if (pipe(fd) == -1)
		return ((pid_t)exec_error(5, ""));
	if ((pid = fork()) < 0)
		return ((pid_t)exec_error(0, "fork"));
	if (pid == 0)
	{
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			return ((pid_t)exec_error(6, "dup2"));
		close(fd[0]);
		link->left->type == PIP ? pipe_fork_child(shell, link->left) : (0);
		link->left->type == CMD ? handle_cmd(shell, link->left, 1) : (0);
		exit(shell->status);
	}
	else if (pid > 0)
	{
		if (dup2(fd[0], STDIN_FILENO) == -1)
			return ((pid_t)exec_error(6, "dup2"));
		close(fd[1]);
		link->right->type == CMD ? handle_cmd(shell, link->right, 1) : (0);
		waitpid(pid, NULL, 0);
	}
	return (pid);
}
