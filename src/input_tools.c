/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hponcet <hponcet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/11 19:38:35 by hponcet           #+#    #+#             */
/*   Updated: 2016/11/20 01:23:16 by hponcet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

size_t	input_len(t_input *input)
{
	size_t	len;
	t_input	*tmp;

	if (!input)
		return (0);
	tmp = input;
	len = 1;
	while (tmp)
	{
		tmp = tmp->next;
		len++;
	}
	return (len);
}

t_input	*char_to_input(char *str)
{
	t_input	*input;
	t_input	*tmp;
	t_input	*new;
	int		i;

	if (!str)
		return (NULL);
	i = 1;
	if (!(input = (t_input*)malloc(sizeof(t_input))))
		ft_put_error(ER_MEM, 1);
	input->c = str[0];
	input->prev = NULL;
	input->next = NULL;
	tmp = input;
	while (str[i])
	{
		if (!(new = (t_input*)malloc(sizeof(t_input))))
			ft_put_error(ER_MEM, 1);
		new->c = str[i++];
		new->prev = tmp;
		tmp->next = new;
		new->next = NULL;
		tmp = new;
	}
	return (input);
}

char	*input_to_char(t_input *input)
{
	t_input	*tmp;
	char	*ret;
	size_t	ilen;
	int		i;

	if (!input)
		return (NULL);
	i = 0;
	ilen = input_len(input);
	tmp = input;
	ret = ft_strnew(ilen);
	while (tmp)
	{
		ret[i] = tmp->c;
		tmp = tmp->next;
		i++;
	}
	return (ret);
}
