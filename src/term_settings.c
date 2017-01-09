/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term_settings.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoehler <fkoehler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/05 18:52:16 by fkoehler          #+#    #+#             */
/*   Updated: 2016/12/13 16:33:10 by hponcet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int	check_termcaps(void)
{
	if (!(tgetstr("do", NULL)))
		return (-1);
	if (!(tgetstr("up", NULL)))
		return (-1);
	if (!(tgetstr("le", NULL)))
		return (-1);
	if (!(tgetstr("nd", NULL)))
		return (-1);
	if (!(tgetstr("cr", NULL)))
		return (-1);
	if (!(tgetstr("cd", NULL)))
		return (-1);
	if (!(tgetstr("cl", NULL)))
		return (-1);
	if (!(tgetstr("sc", NULL)))
		return (-1);
	if (!(tgetstr("rc", NULL)))
		return (-1);
	if (!(tgetstr("UP", NULL)))
		return (-1);
	if (!(tgetstr("DO", NULL)))
		return (-1);
	return (0);
}

void		init_term(t_shell *shell)
{
	int				ret;
	char			*term_name;
	struct winsize	w;

	if (!(term_name = getenv("TERM")))
		ft_put_error(ER_GETTERM, 1);
	if ((ret = tgetent(NULL, term_name)) <= 0)
		ft_put_error(ER_GETENT, 1);
	if ((tcgetattr(STDIN_FILENO, &(shell->term_save))) == -1)
		ft_put_error(ER_GETATTR, 1);
	if ((tcgetattr(STDIN_FILENO, &(shell->termios))) == -1)
		ft_put_error(ER_GETATTR, 1);
	shell->termios.c_lflag &= ~(ICANON | ECHO);
	shell->termios.c_cc[VMIN] = 1;
	shell->termios.c_cc[VTIME] = 0;
	if ((tcsetattr(STDIN_FILENO, TCSADRAIN, &(shell->termios))) == -1)
		ft_put_error(ER_SETATTR, 1);
	if ((ioctl(STDIN_FILENO, TIOCGWINSZ, &w)) < 0)
		ft_put_error(ER_IOCTL, 1);
	shell->col = w.ws_col;
	shell->row = w.ws_row;
	shell->winsize = shell->col * shell->row;
	if (check_termcaps() == -1)
		ft_put_error(ER_TERM_C, 1);
}

void		reload_term(t_shell *shell)
{
	struct winsize	w;

	if ((tcsetattr(STDIN_FILENO, TCSADRAIN, &(shell->termios))) == -1)
		ft_put_error(ER_SETATTR, 1);
	if ((ioctl(STDIN_FILENO, TIOCGWINSZ, &w)) < 0)
		ft_put_error(ER_IOCTL, 1);
	shell->col = w.ws_col;
}

void		restore_term(t_shell *shell)
{
	if ((tcsetattr(STDIN_FILENO, TCSADRAIN, &(shell->term_save))) == -1)
		ft_put_error(ER_SETATTR, 1);
}
