/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoehler <fkoehler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/15 16:24:29 by fkoehler          #+#    #+#             */
/*   Updated: 2016/11/16 17:10:21 by fkoehler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void	put_status_error2(int signum, int fd)
{
	if (signum == 10)
		ft_putstr_fd("42sh: bus error: ", fd);
	else if (signum == 11)
		ft_putstr_fd("42sh: segmentation fault: ", fd);
	else if (signum == 12)
		ft_putstr_fd("42sh: invalid system call: ", fd);
	else if (signum == 13)
		ft_putstr_fd("42sh: write on pipe with no reader: ", fd);
	else if (signum == 14)
		ft_putstr_fd("42sh: alarm: ", fd);
	else if (signum == 15)
		ft_putstr_fd("42sh: terminated: ", fd);
	else if (signum == 24)
		ft_putstr_fd("42sh: cpu limit exceeded: ", fd);
	else if (signum == 25)
		ft_putstr_fd("42sh: file size limit exceeded: ", fd);
	else if (signum == 26)
		ft_putstr_fd("42sh: virtual time alarm: ", fd);
	else if (signum == 27)
		ft_putstr_fd("42sh: profile signal: ", fd);
	else if (signum == 30)
		ft_putstr_fd("42sh: user defined signal 1: ", fd);
	else if (signum == 31)
		ft_putstr_fd("42sh: user defined signal 2: ", fd);
}

static void	put_status_error(int signum, char *cmd)
{
	int		fd;

	fd = STDERR_FILENO;
	if (signum == 1)
		ft_putstr_fd("42sh: hangup: ", fd);
	else if (signum == 2)
		ft_putstr_fd("42sh: interrupt: ", fd);
	else if (signum == 3)
		ft_putstr_fd("42sh: quit: ", fd);
	else if (signum == 4)
		ft_putstr_fd("42sh: illegal instruction: ", fd);
	else if (signum == 5)
		ft_putstr_fd("42sh: trace trap: ", fd);
	else if (signum == 6)
		ft_putstr_fd("42sh: abort: ", fd);
	else if (signum == 7)
		ft_putstr_fd("42sh: emulate instruction executed: ", fd);
	else if (signum == 8)
		ft_putstr_fd("42sh: floating-point exception: ", fd);
	else if (signum == 9)
		ft_putstr_fd("42sh: kill: ", fd);
	else
		put_status_error2(signum, fd);
	ft_putstr_fd(cmd, fd);
	ft_putchar_fd('\n', fd);
}

void		set_status(int *status, char **cmd)
{
	int		signum;
	char	*cmd_str;

	signum = 0;
	if (WIFEXITED(*status))
		*status = WEXITSTATUS(*status);
	else if (WIFSIGNALED(*status))
	{
		signum = WTERMSIG(*status);
		*status = 128 + signum;
		cmd_str = ft_multi_strjoin(cmd, " ");
		put_status_error(signum, cmd_str);
		ft_strdel(&cmd_str);
	}
}
