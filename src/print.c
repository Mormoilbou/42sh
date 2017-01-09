/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoehler <fkoehler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/25 18:19:07 by fkoehler          #+#    #+#             */
/*   Updated: 2016/12/07 21:34:47 by fkoehler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void	cursor_back_to_pos(t_shell *shell, int i)
{
	int	j;

	j = 0;
	while (i > shell->col && (i -= shell->col))
		j++;
	if (j)
		tputs(tgoto(tgetstr("UP", NULL), 0, j), shell->fd[3], &putchar);
	while (i--)
		replace_cursor(shell, 0, 1);
}

static void	cursor_back_overflow(t_shell *shell, t_input *curs)
{
	int		i;
	t_input	*tmp;

	i = input_part_len(curs, shell->curs_pos);
	tmp = shell->curs_pos;
	tputs(tgetstr("rc", NULL), shell->fd[3], &putchar);
	shell->curs_pos = curs;
	while (i--)
	{
		replace_cursor(shell, 1, 0);
		shell->curs_pos = shell->curs_pos->next;
	}
	shell->curs_pos = tmp;
}

static void	print_eol(t_shell *shell, char *buf, t_input *curs, int overflow)
{
	t_input *tmp;
	int		i;

	i = 0;
	tmp = overflow ? shell->input : shell->curs_pos;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	if (((shell->input_len + shell->p_len) % shell->col) == 0 && --i)
	{
		ft_putstr_fd(buf, shell->fd[1]);
		tputs(tgetstr("do", NULL), shell->fd[3], &putchar);
		replace_cursor(shell, 42, 1);
	}
	else
		ft_putstr_fd(buf, shell->fd[1]);
	if (overflow)
		cursor_back_overflow(shell, curs);
	else if (i-- == 0)
		replace_cursor(shell, 1, 1);
	else
		cursor_back_to_pos(shell, i);
}

static void	clear_and_print(t_shell *shell, t_input *curs_pos)
{
	int		i;
	int		overflow;
	char	buf[shell->input_len + 1];
	t_input	*tmp;

	i = 0;
	overflow = 0;
	ft_bzero((void *)buf, shell->input_len + 1);
	if ((lst_len(shell->curs_pos) + shell->col) >= shell->winsize)
	{
		overflow++;
		tmp = shell->input;
		tputs(tgetstr("sc", NULL), shell->fd[3], &putchar);
		tputs(tgetstr("cl", NULL), shell->fd[3], &putchar);
		print_prompt(shell, 0);
	}
	else
	{
		tmp = curs_pos;
		tputs(tgetstr("cd", NULL), shell->fd[3], &putchar);
	}
	while (tmp && (buf[i++] = tmp->c))
		tmp = tmp->next;
	print_eol(shell, buf, curs_pos, overflow);
}

void		print_input(t_shell *shell, t_input *curs_pos, size_t p_len)
{
	if (!curs_pos->next &&
		((get_cursor_x_pos(shell->input, curs_pos, p_len) % shell->col) == 0))
	{
		ft_putchar_fd(curs_pos->c, shell->fd[1]);
		tputs(tgetstr("do", NULL), shell->fd[3], &putchar);
	}
	else if (!curs_pos->next)
		ft_putchar_fd(curs_pos->c, shell->fd[1]);
	else
		clear_and_print(shell, curs_pos);
}
