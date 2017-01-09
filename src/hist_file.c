/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hist_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hponcet <hponcet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/11 16:52:38 by hponcet           #+#    #+#             */
/*   Updated: 2016/12/14 18:04:16 by hponcet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int		hist_proc(t_hist *hist, char *histpath)
{
	t_hist	*tmp;
	char	*input;
	char	*timestamp;
	int		fd;

	if ((fd = open(histpath, O_CREAT | O_WRONLY, 0644)) == -1)
		return (-1);
	tmp = hist;
	while (tmp->next)
		tmp = tmp->next;
	while (tmp)
	{
		input = input_to_char(tmp->input);
		timestamp = ft_itoa(tmp->timestamp);
		ft_putstr_fd(timestamp, fd);
		ft_putstr_fd(": ", fd);
		ft_putendl_fd(input, fd);
		ft_strdel(&input);
		ft_strdel(&timestamp);
		tmp = tmp->prev;
	}
	close(fd);
	return (1);
}

char	*hist_get_histpath(t_shell *shell)
{
	t_env	*homeenv;
	char	*homepath;
	char	*histpath;

	if (!(homeenv = get_env_ptr(shell->env_lst, "HOME")))
		return (NULL);
	homepath = ft_strdup(homeenv->val);
	histpath = ft_joinf("%s/.42sh_history", homepath);
	ft_strdel(&homepath);
	return (histpath);
}

int		hist_to_file(t_shell *shell, t_hist *hist)
{
	char	*histpath;

	histpath = hist_get_histpath(shell);
	hist_proc(hist, histpath);
	ft_strdel(&histpath);
	return (1);
}

void	input_to_hist(t_shell *shell, t_input *input, char *timestamp)
{
	t_hist	*hist;
	t_hist	*tmp;

	if (!(hist = (t_hist *)malloc(sizeof(t_hist))))
		ft_put_error(ER_MEM, 1);
	if (!shell->hist)
	{
		hist->input = input;
		hist->timestamp = (unsigned int)ft_atoi(timestamp);
		hist->prev = NULL;
		hist->next = NULL;
		shell->hist = hist;
		return ;
	}
	else
	{
		tmp = shell->hist;
		while (tmp->prev)
			tmp = tmp->prev;
		hist->input = input;
		hist->timestamp = (unsigned int)ft_atoi(timestamp);
		hist->prev = NULL;
		hist->next = tmp;
		tmp->prev = hist;
	}
}

void	file_to_hist(t_shell *shell)
{
	t_input	*input;
	char	*timestamp;
	char	*histpath;
	char	*buf;
	int		fd;

	histpath = hist_get_histpath(shell);
	if ((fd = open(histpath, O_RDONLY, 0644)) == -1)
	{
		ft_strdel(&histpath);
		return ;
	}
	while (get_next_line(fd, &buf))
	{
		if (ft_cindex(buf, ':') == -1)
			continue ;
		timestamp = ft_strsub(buf, 0, ft_cindex(buf, ':'));
		input = char_to_input(buf + ft_cindex(buf, ':') + 2);
		input_to_hist(shell, input, timestamp);
		ft_strdel(&buf);
		ft_strdel(&timestamp);
	}
	ft_strdel(&histpath);
	close(fd);
}
