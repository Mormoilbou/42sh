/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: MrRobot <mimazouz@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/24 15:44:55 by MrRobot           #+#    #+#             */
/*   Updated: 2016/12/11 17:50:46 by fkoehler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void	sig_handler_read(int signum)
{
	t_shell	*shell;

	shell = get_struct(0);
	if (signum == SIGINT)
		tputs(tgetstr("do", NULL), shell->fd[3], &putchar);
}

void		ft_treat_read(char **argv, char **split, t_env **env, int start)
{
	char	*tmp;
	t_env	*var;
	int		i;

	i = 0;
	while (split[i] != NULL)
	{
		if (argv[start] == NULL)
		{
			tmp = ft_strjoin(var->val, split[i]);
			free(var->val);
			var->val = tmp;
		}
		else if ((var = get_env_ptr(*env, argv[start])) != NULL)
		{
			free(var->val);
			var->val = ft_strdup(split[i]);
			start++;
		}
		i++;
	}
	free_tab(split);
}

static int	ft_check_read_opts(char **argv, t_env **env)
{
	int	j;

	j = 1;
	if (argv[j][1] == '\0')
	{
		ft_putstr(ER_OPT);
		return (1);
	}
	else if (argv[j][1] == 'p')
		return (ft_p_read_opt(argv, env));
	else if (argv[j][1] == 'd')
		return (ft_d_read_opt(argv, env));
	else if (argv[j][1] == 's' && argv[j][2] == 0)
		return (ft_s_read_opt(argv, env));
	else if (argv[j][1] == 'n')
		return (ft_n_read_opt(argv, env, 0));
	else
	{
		ft_putstr(ER_OPT);
		return (1);
	}
	return (0);
}

int			ft_read(char **argv, t_env **env)
{
	char	*line;

	signal(SIGINT, &sig_handler_read);
	line = NULL;
	if (argv[1] == NULL || ft_strcmp(argv[1], "-r") == 0)
	{
		if (get_next_line(0, &line) < 0)
			return (0);
	}
	else if ((argv[1][0] == '-') && (ft_check_read_opts(argv, env) == 1))
	{
		ft_strdel(&line);
		signal(SIGINT, &sig_handler1);
		return (1);
	}
	else if (argv[1][0] != '-')
	{
		get_next_line(0, &line);
		if (line != NULL)
			ft_treat_read(argv, ft_strsplit(line, ' '), env, 1);
	}
	ft_strdel(&line);
	signal(SIGINT, &sig_handler1);
	return (0);
}

int			ft_p_read_opt(char **argv, t_env **env)
{
	char	*line;
	int		i;

	line = NULL;
	i = 1;
	if (argv[i][2] != '\0')
		ft_putstr(argv[i++] + 2);
	else if (argv[++i] != NULL)
		ft_putstr(argv[i++]);
	else
	{
		ft_putstr(ER_ARG);
		return (1);
	}
	get_next_line(0, &line);
	if (argv[i] != NULL)
		ft_treat_read(argv, ft_strsplit(line, ' '), env, i);
	ft_strdel(&line);
	return (0);
}
