/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_options.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: MrRobot <mimazouz@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/27 15:53:40 by MrRobot           #+#    #+#             */
/*   Updated: 2016/12/07 17:39:56 by MrRobot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static char	*ft_read_termset(char limit, char *line)
{
	char	*tmp;
	char	buf[7];

	ft_bzero(buf, 7);
	while (read(0, buf, 6) > 0)
	{
		if (buf[0] == limit || buf[0] == 4 ||
			(buf[0] == 10 && buf[1] == 0 && limit == -1))
			break ;
		if (ft_isprint(buf[0]) == 1 || buf[0] == '\n')
		{
			if (line == NULL)
				line = ft_strdup(&buf[0]);
			else
			{
				tmp = ft_strjoin(line, &buf[0]);
				free(line);
				line = tmp;
			}
			if (limit != -1)
				ft_putchar(buf[0]);
		}
		ft_bzero(buf, 7);
	}
	return (line);
}

static char	*ft_read_nchar(int nb, char *line)
{
	int		i;
	char	*tmp;
	char	buf[7];

	i = 0;
	ft_bzero(buf, 7);
	while (i < nb && read(0, buf, 6) > 0 && buf[0] != 4)
	{
		if (ft_isprint(buf[0]) == 1 || buf[0] == '\n')
		{
			if (line == NULL)
				line = ft_strdup(&buf[0]);
			else
			{
				tmp = ft_strjoin(line, &buf[0]);
				free(line);
				line = tmp;
			}
			ft_putchar(buf[0]);
		}
		ft_bzero(buf, 7);
		i++;
	}
	return (line);
}

int			ft_n_read_opt(char **argv, t_env **env, int nb)
{
	int		i;
	t_shell	*shell;
	char	*line;

	i = 2;
	if (argv[i - 1][2] != '\0')
		nb = ft_atoi(argv[i - 1] + 2);
	else if (argv[i] != NULL)
		nb = ft_atoi(argv[i]);
	if (nb == 0)
	{
		ft_putstr(ER_OPT);
		return (1);
	}
	shell = get_struct(0);
	tcsetattr(STDIN_FILENO, TCSADRAIN, &shell->termios);
	if ((line = ft_read_nchar(nb, NULL)) != NULL)
	{
		if (argv[i] != NULL)
			ft_treat_read(argv, ft_strsplit(line, ' '), env, 2);
		ft_strdel(&line);
	}
	restore_term(shell);
	return (0);
}

int			ft_d_read_opt(char **argv, t_env **env)
{
	t_shell	*shell;
	char	*line;
	char	limit;
	int		i;

	i = 1;
	if (argv[i][2] != '\0')
		limit = argv[i++][2];
	else if (argv[++i] != NULL)
		limit = argv[i++][0];
	else
	{
		ft_putstr(ER_ARG);
		return (1);
	}
	shell = get_struct(0);
	tcsetattr(STDIN_FILENO, TCSADRAIN, &shell->termios);
	if ((line = ft_read_termset(limit, NULL)) != NULL)
	{
		if (argv[i] != NULL)
			ft_treat_read(argv, ft_strtab(line), env, i);
		ft_strdel(&line);
	}
	restore_term(shell);
	return (0);
}

int			ft_s_read_opt(char **argv, t_env **env)
{
	t_shell	*shell;
	char	*line;
	int		i;

	i = 1;
	line = NULL;
	shell = get_struct(0);
	tcsetattr(STDIN_FILENO, TCSADRAIN, &shell->termios);
	if ((line = ft_read_termset(-1, NULL)) != NULL)
	{
		if (argv[i] != NULL)
			ft_treat_read(argv, ft_strsplit(line, ' '), env, 2);
		ft_strdel(&line);
	}
	restore_term(shell);
	return (0);
}
