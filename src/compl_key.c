/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compl_key.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hponcet <hponcet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/14 20:20:22 by hponcet           #+#    #+#             */
/*   Updated: 2016/12/13 11:12:18 by hponcet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int			compl_key_dir(char *buf, t_compl **print, t_shell *shell, int pos)
{
	t_compl		*tmp;

	tmp = print[0];
	if (buf[0] == 27 && buf[1] == 91 && buf[2] == 65)
		tmp = tmp->prev;
	else if (buf[0] == 27 && buf[1] == 91 && buf[2] == 66)
		tmp = tmp->next;
	else if (buf[0] == 27 && buf[1] == 91 && buf[2] == 67)
	{
		move_left(shell);
		return (compl_retstr(*print, shell, pos, 0));
	}
	else if (buf[0] == 27 && buf[1] == 91 && buf[2] == 68)
	{
		move_right(shell);
		return (compl_retstr(*print, shell, pos, 0));
	}
	else if (buf[0] == 27 && buf[1] == 0)
		return (compl_retstr(*print, shell, pos, 1));
	print[0] = tmp;
	return (0);
}

int			compl_key(char *buf, t_compl **print, t_shell *shell, int pos)
{
	t_compl		*tmp;

	tmp = print[0];
	if (tmp->next == tmp)
		return (compl_retstr(tmp, shell, pos, 1));
	if (buf[0] == 10)
		return (compl_retstr(tmp, shell, pos, 1));
	else if (buf[0] == 27 && compl_key_dir(buf, &tmp, shell, pos))
		return (1);
	else if (buf[0] == 9 && buf[1] == 0)
	{
		if (tmp == tmp->next)
			return (compl_retstr(tmp, shell, pos, 0));
		tmp = tmp->next;
	}
	else if (ft_isprint(buf[0]) == 1 && buf[1] == 0)
		return (compl_retstrchar(tmp, shell, pos, buf[0]));
	else if (buf[0] == 127 && buf[1] == 0)
		return (compl_retstr(tmp, shell, pos, 0));
	tputs(tgetstr("rc", 0), 1, putchar);
	ft_putstr(tmp->name + pos);
	tputs(tgetstr("cd", 0), 1, putchar);
	*print = tmp;
	return (0);
}
