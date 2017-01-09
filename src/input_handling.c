/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoehler <fkoehler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/24 15:05:22 by fkoehler          #+#    #+#             */
/*   Updated: 2016/12/11 17:21:29 by fkoehler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int	is_buffer_print(char *buf, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len && buf[i])
	{
		if (!ft_isprint(buf[i]))
			return (0);
		i++;
	}
	return (1);
}

void		store_input(t_shell *shell, char c)
{
	t_input	*new;

	if (!(new = (t_input *)malloc(sizeof(*new))))
		ft_put_error(ER_MEM, 1);
	new->c = c;
	new->prev = shell->curs_pos != NULL ? shell->curs_pos : NULL;
	if (!(shell->input))
	{
		new->next = NULL;
		shell->input = new;
	}
	else if (!(shell->curs_pos))
	{
		new->next = shell->input;
		new->next->prev = new;
		shell->input = new;
	}
	else
	{
		new->next = shell->curs_pos->next;
		new->prev->next = new;
		new->next != NULL ? new->next->prev = new : (0);
	}
	shell->curs_pos = new;
	shell->input_len++;
}

void		delete_input(t_input **lst, t_input *input, t_shell *shell,
			int back)
{
	if (!input->prev && !input->next)
		*lst = NULL;
	else if (!input->prev)
	{
		*lst = input->next;
		(*lst)->prev = NULL;
	}
	else if (!input->next)
		input->prev->next = NULL;
	else
	{
		input->prev->next = input->next;
		input->next->prev = input->prev;
	}
	if (shell)
		shell->input_len--;
	if (shell && back)
		shell->curs_pos = input->prev;
	free(input);
}

void		read_input(t_shell *shell)
{
	int		parse;
	char	buf[BUF_SIZE];
	size_t	buf_len;

	print_prompt(shell, 0);
	while (42)
	{
		ft_bzero((void *)buf, BUF_SIZE);
		if (read(0, buf, BUF_SIZE) == -1)
			ft_put_error(ER_READ, 1);
		signal(SIGINT, &sig_handler1);
		if ((buf_len = ft_strlen(buf)) > 0)
		{
			if ((parse = parse_input(shell, buf, buf_len)) > 0)
				print_prompt(shell, parse);
		}
		signal(SIGINT, &sig_handler);
	}
}

int			parse_input(t_shell *shell, char *buf, size_t buf_len)
{
	if (buf[0] == '!')
		bltn_hsearch(shell);
	else if (is_buffer_print(buf, (size_t)BUF_SIZE))
		insert_read_buf(shell, buf, (size_t)BUF_SIZE);
	else if (buf_len == 3 && buf[0] == 27)
		parse_keys1(shell, buf);
	else if (buf_len == 6)
		parse_keys2(shell, buf);
	else if (buf[0] == 9 && buf[1] == 0 && shell->input && shell->curs_pos
			&& shell->curs_pos->next == NULL)
		compl(shell);
	else
		return (parse_keys3(shell, buf, buf_len));
	return (0);
}
