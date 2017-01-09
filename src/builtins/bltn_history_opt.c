/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_history_opt.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hponcet <hponcet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/15 16:43:39 by hponcet           #+#    #+#             */
/*   Updated: 2016/11/20 14:45:22 by hponcet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int		bltn_hist_checkopt(char *cmd)
{
	int		i;

	i = 0;
	if (!cmd)
		return (-1);
	while (cmd[i])
	{
		if ((cmd[i] >= ' ' && cmd[i] <= '/')
				|| (cmd[i] >= ':' && cmd[i] <= '~'))
			return (0);
		i++;
	}
	return (1);
}

t_hist	*bltn_hist_searchstr(char *cmd, t_shell *shell, int opt)
{
	t_hist	*hist;
	char	*input;

	hist = shell->hist;
	while (hist)
	{
		input = input_to_char(hist->input);
		if (opt == 0 && ft_strinstr(input, cmd) == 1
				&& ft_strncmp(input, "history ", 8) != 0)
		{
			ft_strdel(&input);
			break ;
		}
		if (opt == 1 && ft_strncmp(input, cmd, ft_strlen(cmd)) == 0)
		{
			ft_strdel(&input);
			break ;
		}
		ft_strdel(&input);
		hist = hist->prev;
	}
	return (hist);
}

t_hist	*bltn_hist_searchindex(char *cmd, t_shell *shell)
{
	t_hist	*hist;
	int		index;
	int		i;

	i = 1;
	index = ft_atoi(cmd);
	hist = shell->hist;
	while (hist->prev)
		hist = hist->prev;
	while (hist->next && i < index)
	{
		hist = hist->next;
		i++;
	}
	return (hist);
}
