/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoehler <fkoehler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/14 20:01:15 by fkoehler          #+#    #+#             */
/*   Updated: 2016/12/01 21:25:29 by fkoehler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static char	*get_relative_path(t_env *home, t_env *pwd)
{
	int		i;
	char	*prompt;

	i = 0;
	while (home->val[i] && pwd->val[i]
			&& home->val[i] == pwd->val[i])
		i++;
	if (!home->val[i])
		prompt = ft_strjoin("~", (pwd->val + i));
	else if (!pwd->val[i] || access(pwd->val, F_OK) == 0)
		prompt = ft_strdup(pwd->val);
	else
		prompt = ft_strdup("$");
	return (prompt);
}

char		*get_prompt(t_env *env_lst)
{
	int			i;
	static char	*prompt = NULL;
	char		*tmp;
	t_env		*home;
	t_env		*pwd;

	i = 0;
	tmp = NULL;
	if (prompt != NULL)
		free(prompt);
	home = get_env_ptr(env_lst, "HOME");
	pwd = get_env_ptr(env_lst, "PWD");
	if (!pwd || !pwd->val || !pwd->val[0])
		prompt = ft_strdup("$");
	else if (!home || !home->val || !home->val[0])
		prompt = ft_strdup(pwd->val);
	else
		prompt = get_relative_path(home, pwd);
	if ((i = ft_strchr_index(prompt, '\n')) != -1)
	{
		tmp = ft_replace_str(prompt, i, 1, "\\n");
		free(prompt);
		prompt = tmp;
	}
	return (prompt);
}

char		*get_special_prompt(char c)
{
	static char	*prompt = NULL;

	if (prompt != NULL)
		free(prompt);
	if (c == '\'')
		prompt = ft_strdup("quote> ");
	else if (c == '"')
		prompt = ft_strdup("dquote> ");
	else if (c == '`')
		prompt = ft_strdup("bquote> ");
	else if (c == ']')
		prompt = ft_strdup("hook> ");
	else if (c == ')')
		prompt = ft_strdup("parenth> ");
	else if (c == '}')
		prompt = ft_strdup("bracket> ");
	else if (c == '|')
		prompt = ft_strdup("pipe> ");
	else if (c == '\\')
		prompt = ft_strdup("> ");
	else if (c == '&')
		prompt = ft_strdup("cmdand> ");
	else if (c == '+')
		prompt = ft_strdup("cmdor> ");
	return (prompt);
}

void		print_prompt(t_shell *shell, int special_prompt)
{
	int		fd;
	char	*prompt;

	fd = shell->fd[3];
	prompt = NULL;
	if (special_prompt > 1)
	{
		prompt = get_special_prompt((char)special_prompt);
		ft_putstr_fd(prompt, fd);
		shell->p_len = ft_strlen(prompt);
	}
	else
	{
		prompt = get_prompt(shell->env_lst);
		ft_putstr_fd(CYAN, fd);
		ft_putchar_fd('[', fd);
		ft_putstr_fd(prompt, fd);
		ft_putstr_fd("]-> ", fd);
		ft_putstr_fd(OFF, fd);
		shell->p_len = ft_strlen(prompt) + 5;
	}
}
