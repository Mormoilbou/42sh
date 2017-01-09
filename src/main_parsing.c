/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoehler <fkoehler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/20 17:50:57 by fkoehler          #+#    #+#             */
/*   Updated: 2016/12/09 21:41:27 by MrRobot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int	check_input_form(t_shell *shell)
{
	char			c;
	t_input			*tmp;

	c = 0;
	if (shell->input_save)
	{
		shell->input_len += lst_len(shell->input_save);
		tmp = get_last_elem(shell->input_save);
		tmp->next = shell->input;
		if (shell->input)
			shell->input->prev = tmp;
		shell->input = shell->input_save;
	}
	if ((c = valid_input(shell->input)) > 0)
	{
		c == '\\' ? delete_input(&(shell->input), shell->curs_pos, shell, 0)
		: store_input(shell, '\n');
		shell->input_save = shell->input;
		shell->input = NULL;
		shell->input_len = 0;
		shell->curs_pos = NULL;
	}
	else
		shell->input_save = NULL;
	return ((int)c);
}

int			check_input(t_shell *shell)
{
	char	ret;

	ret = 1;
	if ((!shell->input && !shell->input_save) || lst_is_empty(shell->input))
		return (1);
	if (check_separators(shell->input, 1) == -1)
	{
		free_tmp_inputs(shell, 0);
		return (1);
	}
	if ((ret = check_input_form(shell)) != 0)
	{
		ret > 0 ? free_tmp_inputs(shell, 0) : free_tmp_inputs(shell, 1);
		return (ret == -1 ? 1 : (int)ret);
	}
	return (0);
}

int			check_btree(t_btree *link)
{
	if (link->type != CMD)
	{
		if (link->left && check_btree(link->left) > 0)
			return (1);
		if (link->right && check_btree(link->right) > 0)
			return (1);
	}
	else if ((strchr_redir(link)) == -1)
		return (1);
	return (0);
}
