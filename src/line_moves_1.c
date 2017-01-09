/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_moves_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoehler <fkoehler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/25 17:27:02 by fkoehler          #+#    #+#             */
/*   Updated: 2016/12/11 17:22:11 by fkoehler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int	get_input_len(t_shell *shell)
{
	if (!shell->curs_pos && !shell->input)
		return (-1);
	if (!shell->curs_pos && (shell->curs_pos = shell->input))
		replace_cursor(shell, 0, 0);
	return (lst_len(shell->curs_pos) - 1);
}

int			move_left(t_shell *shell)
{
	if (!(shell->input) || !(shell->curs_pos))
		return (-1);
	replace_cursor(shell, 0, 1);
	shell->curs_pos = shell->curs_pos->prev;
	return (0);
}

int			move_right(t_shell *shell)
{
	if (!(shell->input) || (shell->curs_pos && !(shell->curs_pos->next)))
		return (-1);
	if (!(shell->curs_pos))
		shell->curs_pos = shell->input;
	else
		shell->curs_pos = shell->curs_pos->next;
	replace_cursor(shell, 0, 0);
	return (0);
}

int			move_line_start(t_shell *shell)
{
	int		i;
	int		j;

	if (!shell->curs_pos)
		return (-1);
	i = input_part_len(shell->input, shell->curs_pos);
	j = 0;
	while (i > shell->col && (i -= shell->col))
		j++;
	if (j)
	{
		tputs(tgoto(tgetstr("UP", NULL), 0, j), shell->fd[3], &putchar);
		j *= shell->col;
		while (j--)
			shell->curs_pos = shell->curs_pos->prev;
	}
	while (i--)
	{
		replace_cursor(shell, 0, 1);
		shell->curs_pos = shell->curs_pos->prev;
	}
	return (0);
}

int			move_line_end(t_shell *shell)
{
	int		i;
	int		j;
	int		k;

	j = 0;
	if ((i = get_input_len(shell)) < 0)
		return (-1);
	while (i > shell->col && (i -= shell->col))
		j++;
	if (j && (k = j) && (j *= shell->col))
	{
		if ((k > shell->row - 2) && (i >= (shell->col - (int)shell->p_len - 1)))
			k = shell->row - 2;
		else if (k > shell->row - 2)
			k = shell->row - 1;
		tputs(tgoto(tgetstr("DO", NULL), 0, k), shell->fd[3], &putchar);
		while (j--)
			shell->curs_pos = shell->curs_pos->next;
	}
	while (i--)
	{
		shell->curs_pos = shell->curs_pos->next;
		replace_cursor(shell, 0, 0);
	}
	return (0);
}
