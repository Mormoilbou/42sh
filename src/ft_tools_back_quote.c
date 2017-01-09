/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tools_back_quote.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: MrRobot <mimazouz@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/11 17:44:09 by MrRobot           #+#    #+#             */
/*   Updated: 2016/12/14 11:24:32 by MrRobot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int		ft_prepare_cmd(t_shell *shell, t_btree *tree)
{
	t_env	*new_env;
	t_env	*tmp;
	int		ret;
	pid_t	father;

	ret = 0;
	tmp = shell->env_lst;
	new_env = NULL;
	dup_env_lst(shell->env_lst, &new_env);
	shell->env_lst = new_env;
	father = fork();
	if (father == 0)
	{
		ret = ft_launch_cmd(shell, tree);
		free_env_lst(&shell->env_lst);
		shell->env_lst = tmp;
		exit(EXIT_SUCCESS);
	}
	else
	{
		wait(0);
		return (ret);
	}
}

size_t	ft_list_size_index(t_input *start, t_input *end)
{
	size_t size;

	size = 0;
	while (start != end)
	{
		start = start->next;
		size++;
	}
	size++;
	return (size);
}

void	ft_lst_del(t_input **input, t_input *start, t_input *end)
{
	t_input	*tmp;

	while (start != end)
	{
		tmp = start->next;
		delete_input(input, start, NULL, 0);
		start = tmp;
	}
	if (start != NULL)
		delete_input(input, start, NULL, 0);
}

t_input	*ft_new_link(char c)
{
	t_input *new;

	new = malloc(sizeof(t_input));
	new->c = c;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	ft_input_add(t_input **curs, t_input **tmp, char c)
{
	t_input	*new;

	new = ft_new_link(c);
	if (*tmp == NULL)
	{
		*tmp = new;
		*curs = *tmp;
	}
	else if (*curs == NULL)
	{
		*curs = new;
		(*curs)->next = *tmp;
		(*tmp)->prev = *curs;
		*tmp = *curs;
	}
	else
	{
		new->prev = *curs;
		new->next = (*curs)->next;
		if ((*curs)->next != NULL)
			(*curs)->next->prev = new;
		(*curs)->next = new;
		*curs = (*curs)->next;
	}
}
