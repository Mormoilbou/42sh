/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_back_quote.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: MrRobot <mimazouz@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/17 18:59:56 by MrRobot           #+#    #+#             */
/*   Updated: 2016/12/11 18:00:21 by MrRobot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int		ft_check_input(t_input *input)
{
	char	ret;
	t_input	*tmp;

	tmp = get_last_elem(input);
	ret = 0;
	if (check_separators(input, 1) == -1)
	{
		free_input_list(&input, NULL);
		return (1);
	}
	if ((ret = valid_input(input)) != 0)
	{
		free_input_list(&input, NULL);
		return (ret == -1 ? 1 : cmd_error(0, ret, NULL));
	}
	if (tmp->c == '`')
	{
		free_input_list(&input, NULL);
		return (1);
	}
	free_input_list(&input, NULL);
	return (0);
}

static void		ft_output_insert(t_input **curs, int fd, t_input **tmp)
{
	char	*line;
	int		i;
	int		ret;

	line = NULL;
	ret = get_next_line(fd, &line);
	while (ret == 1)
	{
		i = 0;
		while (line[i] != 0)
		{
			ft_input_add(curs, tmp, line[i]);
			i++;
		}
		ft_strdel(&line);
		if ((ret = get_next_line(fd, &line)) == 1)
			ft_input_add(curs, tmp, ' ');
	}
}

t_input			*ft_tbq(t_shell *shell, t_input **c, char *str, t_input **tmp)
{
	t_btree	*tree;
	t_input	*input;
	int		fd;

	input = char_to_input(str);
	if (ft_check_input(input) != 0)
	{
		ft_strdel(&str);
		return (*c == NULL ? *tmp : *c);
	}
	fd = open("/tmp/ft_back_quote.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
	shell->fd[1] = fd;
	tree = store_cmd(str);
	if (ft_prepare_cmd(shell, tree) == 1)
	{
		close_and_reset_fd(shell->fd);
		close(fd);
		return (*c == NULL ? *tmp : *c);
	}
	close(fd);
	close_and_reset_fd(shell->fd);
	fd = open("/tmp/ft_back_quote.txt", O_RDONLY);
	ft_output_insert(c, fd, tmp);
	close(fd);
	return (*c);
}

static char		*ft_lst_to_str_index(t_input *start, t_input *end)
{
	char	*cmd;
	int		i;

	i = 0;
	if (start->c == '`')
		return (NULL);
	cmd = ft_strnew(ft_list_size_index(start, end));
	while (start != end)
	{
		cmd[i] = start->c;
		start = start->next;
		i++;
	}
	cmd[i] = start->c;
	return (cmd);
}

char			*ft_back_q(t_shell *shell, t_input *start, char *cmd)
{
	t_input	*end;
	t_input	*tmp;
	t_input	*curs;

	tmp = start;
	while (start != NULL)
	{
		if (start->c == '`' && (end = start->next))
		{
			while (end->c != '`')
				end = end->next;
			curs = start->prev;
			cmd = ft_lst_to_str_index(start->next, end->prev);
			ft_lst_del(&tmp, start, end);
			if (cmd == NULL)
				start = curs == NULL ? tmp : curs;
			else
				start = ft_tbq(shell, &curs, cmd, &tmp);
		}
		else
			start = start->next;
	}
	cmd = lst_to_str(tmp);
	ft_lst_del(&tmp, tmp, NULL);
	return (cmd);
}
