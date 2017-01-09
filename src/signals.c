/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoehler <fkoehler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/17 15:51:02 by fkoehler          #+#    #+#             */
/*   Updated: 2016/12/11 17:24:26 by fkoehler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void	sig_int_handler(t_shell *shell)
{
	if (shell->input || shell->input_save)
	{
		move_line_end(shell);
		free_tmp_inputs(shell, 1);
	}
	tputs(tgetstr("do", NULL), shell->fd[3], &putchar);
	print_prompt(shell, 0);
}

static void	sig_winch_handler(t_shell *shell)
{
	t_input			*curs_pos_save;
	struct winsize	w;

	if ((ioctl(STDIN_FILENO, TIOCGWINSZ, &w)) < 0)
		ft_put_error(ER_IOCTL, 1);
	shell->col = w.ws_col;
	shell->row = w.ws_row;
	shell->winsize = shell->col * shell->row;
	if (shell->input)
	{
		tputs(tgetstr("cl", NULL), shell->fd[3], &putchar);
		print_prompt(shell, 0);
		curs_pos_save = shell->curs_pos;
		shell->curs_pos = shell->input;
		print_input(shell, shell->input, shell->p_len);
		if (curs_pos_save == NULL)
			move_left(shell);
		while (shell->curs_pos && shell->curs_pos != curs_pos_save)
		{
			shell->curs_pos = shell->curs_pos->next;
			replace_cursor(shell, 0, 0);
		}
	}
}

void		sig_handler(int signum)
{
	char	suspend;
	t_shell *shell;

	suspend = 26;
	shell = get_struct(0);
	if (signum == SIGINT)
		sig_int_handler(shell);
	else if (signum == SIGTSTP)
	{
		move_line_end(shell);
		signal(SIGTSTP, SIG_DFL);
		restore_term(shell);
		ioctl(0, TIOCSTI, &suspend);
	}
	else if (signum == SIGCONT)
	{
		reload_term(shell);
		signal(SIGTSTP, &sig_handler);
		if (!shell->tree)
			print_prompt(shell, 0);
		free_tmp_inputs(shell, 1);
	}
	else if (signum == SIGWINCH)
		sig_winch_handler(shell);
}

void		sig_handler1(int signum)
{
	t_shell *shell;

	shell = get_struct(0);
	if (signum == SIGINT)
	{
		free_tmp_inputs(shell, 1);
		tputs(tgetstr("do", NULL), shell->fd[3], &putchar);
	}
}

void		set_sig_handler(void)
{
	signal(SIGINT, &sig_handler);
	signal(SIGTSTP, &sig_handler);
	signal(SIGCONT, &sig_handler);
	signal(SIGWINCH, &sig_handler);
}
