/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_hsearch.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hponcet <hponcet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/18 17:49:22 by hponcet           #+#    #+#             */
/*   Updated: 2016/11/20 14:14:14 by hponcet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void		bltn_hsearch_display(t_shell *shell, t_hist *hist, int c)
{
	char		*str;
	char		*ostr;
	char		*tmp;
	int			i;

	str = NULL;
	if (!hist)
		return ;
	str = input_to_char(hist->input);
	ostr = input_to_char(shell->input);
	i = ft_cindex_rev(ostr, '!');
	tmp = ft_strsub(ostr, 0, i);
	free_tmp_inputs(shell, 0);
	if (c == '\n')
	{
		ft_putchar(c);
		print_prompt(shell, 0);
	}
	compl_addstr(shell, tmp);
	tputs(tgetstr("rc", 0), 1, putchar);
	tputs(tgetstr("cd", 0), 1, putchar);
	compl_addstr(shell, str);
	ft_strdel(&str);
	ft_strdel(&ostr);
	ft_strdel(&tmp);
}

int			bltn_hsearch_ret(t_shell *shell, int c)
{
	char	*srch;
	char	*str;
	t_hist	*hist;
	int		i;

	hist = NULL;
	str = input_to_char(shell->input);
	i = ft_cindex_rev(str, '!');
	srch = str + i + 1;
	if (bltn_hist_checkopt(srch))
		hist = bltn_hist_searchindex(srch, shell);
	else
		hist = bltn_hist_searchstr(srch, shell, 1);
	bltn_hsearch_display(shell, hist, c);
	ft_strdel(&str);
	return (1);
}

int			bltn_hsearch_addchar(t_shell *shell, char *buf)
{
	store_input(shell, buf[0]);
	ft_putchar(buf[0]);
	return (0);
}

int			bltn_hsearch_key(t_shell *shell, char *buf)
{
	if (ft_isprint(buf[0]) == 1 && buf[1] == 0 && buf[0] != ' ')
		return (bltn_hsearch_addchar(shell, buf));
	else if (buf[0] == 27 && buf[1] == 91 && buf[2] == 68)
		move_left(shell);
	else if (buf[0] == 27 && buf[1] == 91 && buf[2] == 67)
		move_right(shell);
	else if (buf[0] == 127)
	{
		backspace(shell);
		return (1);
	}
	else if (buf[0] == 27 && buf[1] == 91
			&& buf[2] == 51 && buf[3] == 126)
		del(shell);
	else if (buf[0] == '\n' || buf[0] == '\t' || buf[0] == ' ')
		return (bltn_hsearch_ret(shell, buf[0]));
	return (0);
}

void		bltn_hsearch(t_shell *shell)
{
	char	*buf;

	buf = ft_strnew(4);
	tputs(tgetstr("sc", 0), 1, putchar);
	ft_putchar('!');
	store_input(shell, '!');
	signal(SIGINT, SIG_IGN);
	while (42)
	{
		ft_bzero(buf, 4);
		read(0, buf, 4);
		if ((buf[0] == 27 || buf[0] == 4) && buf[1] == 0 &&
				compl_initprompt(shell, buf[0]))
			break ;
		if (bltn_hsearch_key(shell, buf))
			break ;
	}
	ft_strdel(&buf);
}
