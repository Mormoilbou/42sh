/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoehler <fkoehler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/11 14:13:19 by fkoehler          #+#    #+#             */
/*   Updated: 2016/12/11 16:00:29 by fkoehler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static char	**move_cmd_start(char **cmd)
{
	int		i;
	char	**new_cmd;

	i = 0;
	while (cmd[i])
		i++;
	if (i == 1)
	{
		free_tab(cmd);
		return (NULL);
	}
	if (!(new_cmd = (char**)malloc(sizeof(char*) * i)))
		ft_put_error(ER_MEM, 1);
	i = 0;
	while (cmd[i++])
		new_cmd[i - 1] = cmd[i];
	ft_strdel(cmd);
	free(cmd);
	return (new_cmd);
}

char		**parse_cmd(t_env *env_lst, t_btree *link)
{
	int		i;
	char	*tmp;
	char	**cmd_tab;

	i = 0;
	tmp = remove_cmd_redir(ft_strtrim(link->str), link->redir);
	free(link->str);
	link->str = tmp;
	if (!(cmd_tab = strsplit_args(link->str)) || !cmd_tab[0])
		return (NULL);
	while (cmd_tab[i])
	{
		if (i == 0 && (strchr_outside_quotes(cmd_tab[i], '=') != -1))
		{
			if (set_local_variable(env_lst, &cmd_tab[i]) != 0)
				i++;
			else if (!(cmd_tab = move_cmd_start(cmd_tab)))
				return (NULL);
			continue;
		}
		else
			cmd_tab[i] = interpret_cmd_arg(cmd_tab[i]);
		i++;
	}
	return (cmd_tab);
}

int			handle_input(t_shell *shell)
{
	int		ret;
	char	*cmd_str;

	cmd_str = NULL;
	move_line_end(shell);
	tputs(tgetstr("do", NULL), shell->fd[3], &putchar);
	if ((ret = check_input(shell)) > 0)
		return (ret);
	if (!(hist_checkdouble(shell)))
		shell->hist = store_hist(shell);
	shell->input_len = lst_len(shell->input);
	cmd_str = lst_to_str(shell->input);
	shell->tree = store_cmd(cmd_str);
	free_tmp_inputs(shell, 1);
	ft_launch_cmd(shell, shell->tree);
	return (0);
}

int			ft_launch_cmd(t_shell *shell, t_btree *tree)
{
	if (check_btree(tree) > 0)
	{
		free_btree(tree);
		tree = NULL;
		return (1);
	}
	restore_term(shell);
	signal(SIGTSTP, SIG_DFL);
	signal(SIGINT, &sig_handler1);
	handle_btree(shell, tree);
	free_btree(tree);
	tree = NULL;
	reload_term(shell);
	signal(SIGTSTP, &sig_handler);
	return (0);
}
