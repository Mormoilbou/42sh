/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_interpreting.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoehler <fkoehler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/21 15:21:55 by fkoehler          #+#    #+#             */
/*   Updated: 2016/12/09 20:20:12 by fkoehler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static char	*replace_special_char(char *s, int quote)
{
	int		i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\\')
			i += replace_backslash(&s, i, quote);
		else if (s[i] == '~' && !is_chr_escaped(s, i))
			i += replace_tilde(&s, i, quote);
		else if (s[i] == '$' && (!is_chr_escaped(s, i)) && s[i + 1])
		{
			if (s[i + 1] == '?' || s[i + 1] == '$')
				i += replace_process_value(&s, i, quote);
			else
			{
				s = str_replace_var(s, i, quote);
				if (s[i])
					i++;
			}
		}
		else
			i++;
	}
	return (s);
}

char		*interpret_cmd_arg(char *cmd_arg)
{
	int		i;
	int		quote;
	char	*ret;
	char	**arg_tab;

	i = 0;
	ret = NULL;
	arg_tab = str_subsplit_arg(cmd_arg);
	while (arg_tab[i])
	{
		if ((quote = is_str_quoted(arg_tab[i])))
			arg_tab[i] = strdup_remove_quotes(arg_tab[i]);
		arg_tab[i] = replace_special_char(arg_tab[i], quote);
		i++;
	}
	ret = ft_multi_strjoin(arg_tab, "");
	free_tab(arg_tab);
	free(cmd_arg);
	return (ret);
}

int			set_local_variable(t_env *env_lst, char **cmd)
{
	*cmd = interpret_cmd_arg(*cmd);
	if (check_env_var(*cmd, NULL) == -1)
		return (1);
	set_shell_var(env_lst, *cmd, 1);
	return (0);
}

char		*remove_cmd_redir(char *cmd, t_redir *redir)
{
	int		start;
	char	*tmp_cmd;
	t_redir	*tmp;

	if (!(tmp = redir))
		return (cmd);
	while (tmp)
	{
		start = ft_strstr_index(cmd, tmp->s);
		tmp_cmd = ft_replace_str(cmd, start, ft_strlen(tmp->s), "");
		free(cmd);
		cmd = tmp_cmd;
		tmp = tmp->next;
	}
	return (cmd);
}
