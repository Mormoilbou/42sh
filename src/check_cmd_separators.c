/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input_separators.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoehler <fkoehler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/22 15:54:24 by fkoehler          #+#    #+#             */
/*   Updated: 2016/12/09 20:09:52 by fkoehler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int	check_separator_position(t_input *input, int reverse)
{
	if (reverse)
	{
		if (!(input = input->prev))
			return (-1);
		while (input && input->c != ';' && input->c != '|' && input->c != '&')
		{
			if (!ft_isspace(input->c) && input->c != '>' && input->c != '<')
				return (0);
			input = input->prev;
		}
	}
	else
	{
		if (!(input = input->next))
			return (-1);
		while (input && input->c != ';' && input->c != '|' && input->c != '&')
		{
			if (!ft_isspace(input->c))
				return (0);
			input = input->next;
		}
		if (input)
			return (1);
	}
	return (-1);
}

char		check_separators(t_input *input, int reverse)
{
	char	c;
	int		ret;

	ret = 0;
	while (input)
	{
		if ((input->c == '|' || input->c == '&') && (c = input->c))
		{
			if (reverse && input->prev && input->prev->c == c)
				;
			else if (!reverse && input->next && input->next->c == c)
				;
			else if ((ret = check_separator_position(input, reverse)) != 0)
			{
				if (!reverse && ret == -1 && input->prev && input->prev->c == c)
					return (c == '|' ? '+' : '&');
				if (!reverse && ret == -1 && c == '|')
					return (c);
				return (cmd_error(0, c, NULL));
			}
		}
		input = input->next;
	}
	return (0);
}
