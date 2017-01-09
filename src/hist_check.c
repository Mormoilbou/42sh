/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hist_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hponcet <hponcet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/15 04:37:52 by hponcet           #+#    #+#             */
/*   Updated: 2016/11/16 20:17:24 by hponcet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int			hist_checkdouble(t_shell *shell)
{
	t_hist	*thist;
	char	*tmp;
	char	*tmp2;
	int		i;

	i = 0;
	if (!(shell->hist))
		return (i);
	thist = shell->hist;
	tmp = input_to_char(shell->input);
	while (thist->next)
		thist = thist->next;
	tmp2 = input_to_char(thist->input);
	if (ft_strcmp(tmp, tmp2) == 0)
		i = 1;
	ft_strdel(&tmp);
	ft_strdel(&tmp2);
	return (i);
}
