/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compl_display.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hponcet <hponcet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/14 11:20:47 by hponcet           #+#    #+#             */
/*   Updated: 2016/12/12 13:39:25 by hponcet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void		compl_addstr(t_shell *shell, char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		store_input(shell, str[i]);
		ft_putchar(str[i++]);
	}
}

int			compl_countfile(t_compl *print)
{
	t_compl	*tmp;
	int		count;

	count = 0;
	tmp = print;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

int			compl_initprompt(t_shell *shell, int i)
{
	if (i == 27)
	{
		tputs(tgetstr("cr", 0), 1, putchar);
		tputs(tgetstr("ce", 0), 1, putchar);
		print_prompt(shell, 1);
		print_input(shell, shell->input, shell->p_len);
		return (1);
	}
	if (i == 4)
		tputs(tgetstr("do", 0), 1, putchar);
	if (i == 0)
	{
		tputs(tgetstr("do", 0), 1, putchar);
		return (1);
	}
	print_prompt(shell, 1);
	free_tmp_inputs(shell, 0);
	tputs(tgetstr("cd", 0), 1, putchar);
	return (1);
}

void		compl_display(t_shell *shell, t_compl *print, char *find)
{
	char	*buf;
	t_compl	*tmp;
	int		pos;

	pos = ft_strlen(find);
	tputs(tgetstr("sc", 0), 1, putchar);
	tmp = print;
	ft_putstr(print->name + pos);
	tputs(tgetstr("ce", 0), 1, putchar);
	buf = ft_strnew(4);
	signal(SIGINT, SIG_IGN);
	while (42)
	{
		if (compl_key(buf, &tmp, shell, pos) == 1)
			break ;
		ft_bzero(buf, 4);
		read(0, buf, 4);
		if ((buf[0] == 27 || buf[0] == 4) && buf[1] == 0
				&& compl_initprompt(shell, buf[0]))
			break ;
	}
	compl_delchain(print);
	ft_strdel(&buf);
}
