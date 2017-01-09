/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_check_input.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoehler <fkoehler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/24 16:11:42 by fkoehler          #+#    #+#             */
/*   Updated: 2016/12/11 17:01:38 by fkoehler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int	is_input_char_escaped(t_input *input)
{
	int	backslash;

	backslash = 0;
	while ((input = input->prev) && input->c == '\\')
		backslash++;
	return (backslash % 2);
}

static char	is_quote_closed(t_input *tmp, char c)
{
	tmp = tmp->next;
	while (tmp)
	{
		if (tmp->c == c && !is_input_char_escaped(tmp))
			return (0);
		tmp = tmp->next;
	}
	return (c);
}

static char	is_bracket_closed(t_input *tmp, char c)
{
	size_t	count;
	char	d;

	count = 1;
	d = (c == '(') ? c + 1 : c + 2;
	while ((tmp = tmp->next))
	{
		if (ft_isquote(tmp->c) && !is_input_char_escaped(tmp))
		{
			if ((d = is_quote_closed(tmp, tmp->c)) != 0)
				return (d);
			d = tmp->c;
			tmp = tmp->next;
			while (tmp && (tmp->c != d ||
				(tmp->c == d && is_input_char_escaped(tmp))))
				tmp = tmp->next;
			d = (c == '(') ? c + 1 : c + 2;
		}
		else if (tmp->c == c && !is_input_char_escaped(tmp))
			count++;
		else if (tmp->c == d && !is_input_char_escaped(tmp) && (--count == 0))
			return (0);
	}
	return (d);
}

static char	check_quotes(t_input **input, char c, t_input *tmp)
{
	if (ft_isquote(tmp->c) && !is_input_char_escaped(tmp))
	{
		if ((c = is_quote_closed(tmp, tmp->c)) != 0)
			return (c);
		c = tmp->c;
		tmp = tmp->next;
		while (tmp && (tmp->c != c ||
			(tmp->c == c && is_input_char_escaped(tmp))))
			tmp = tmp->next;
	}
	else if ((tmp->c == '[' || tmp->c == '{' || tmp->c == '(')
			&& !is_input_char_escaped(tmp))
	{
		if ((c = is_bracket_closed(tmp, tmp->c)) != 0)
			return (c);
		c = (tmp->c == '(') ? tmp->c + 1 : tmp->c + 2;
		while ((tmp = tmp->next) &&
				(tmp->c != c || (tmp->c == c && is_input_char_escaped(tmp))))
		{
			if (ft_isquote(tmp->c) && !is_input_char_escaped(tmp))
				c = tmp->c;
		}
	}
	*input = tmp;
	return (0);
}

char		valid_input(t_input *input)
{
	char	ret;
	t_input	*tmp;
	t_input	*sub_tmp;

	ret = 0;
	tmp = input;
	while (tmp)
	{
		sub_tmp = tmp;
		if ((ret = check_quotes(&tmp, tmp->c, sub_tmp)) != 0)
			return (ret);
		if (tmp)
			tmp = tmp->next;
	}
	tmp = get_last_elem(input);
	if (tmp->c == '\\' && !is_input_char_escaped(tmp))
		return ('\\');
	return (check_separators(input, 0));
}
