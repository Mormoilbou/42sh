/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hist_storage.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoehler <fkoehler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/10 18:54:12 by fkoehler          #+#    #+#             */
/*   Updated: 2016/12/11 17:20:13 by fkoehler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void		store_hist_input(char c, t_hist *hist)
{
	t_input *new;
	t_input *tmp;

	if (!(new = (t_input *)malloc(sizeof(*new))))
		ft_put_error(ER_MEM, 1);
	new->c = (c == '\n') ? ' ' : c;
	new->prev = NULL;
	new->next = NULL;
	if (!(tmp = hist->input))
		hist->input = new;
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
		new->prev = tmp;
	}
}

static t_hist	*new_hist(void)
{
	t_hist	*new;
	time_t	t;

	time(&t);
	if (!(new = (t_hist *)malloc(sizeof(*new))))
		ft_put_error(ER_MEM, 1);
	new->prev = NULL;
	new->next = NULL;
	new->input = NULL;
	new->timestamp = (unsigned int)t;
	return (new);
}

t_hist			*store_hist(t_shell *shell)
{
	t_hist	*new;
	t_hist	*tmp1;
	t_input	*tmp2;

	new = new_hist();
	tmp2 = shell->input;
	while (tmp2)
	{
		store_hist_input(tmp2->c, new);
		tmp2 = tmp2->next;
	}
	if ((tmp1 = shell->hist))
	{
		while (tmp1->next)
			tmp1 = tmp1->next;
		tmp1->next = new;
		new->prev = tmp1;
	}
	hist_to_file(shell, new);
	shell->hist_end = 1;
	return (new);
}
