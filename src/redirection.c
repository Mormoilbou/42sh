/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoehler <fkoehler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/07 16:28:01 by fkoehler          #+#    #+#             */
/*   Updated: 2016/12/14 18:13:10 by fkoehler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int	redir_exec(t_shell *shell, char **cmd)
{
	char	**env_array;

	env_array = NULL;
	if (dup_std_fd(shell->fd) != 0)
		return (-1);
	if (is_builtin(cmd[0]))
		builtins_cmd(cmd, shell->env_lst, shell);
	else
	{
		env_array = env_lst_to_array(shell->env_lst);
		binary_cmd(cmd, env_array, shell->env_lst, shell->hash_bin);
		free_tab(env_array);
	}
	return (0);
}

static int	open_file(char *file, int redir_type, int *fd, int stdin_copy)
{
	int		file_fd;

	file_fd = -1;
	if (redir_type == REDIR)
		file_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir_type == DREDIR)
		file_fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (redir_type == BREDIR)
		file_fd = open(file, O_RDONLY);
	else if (redir_type == HEREDOC)
	{
		dup2(stdin_copy, STDIN_FILENO);
		signal(SIGINT, &sig_handler_heredoc);
		signal(SIGTSTP, &sig_handler_heredoc);
		file_fd = fill_heredoc(file, fd);
		signal(SIGINT, &sig_handler1);
		signal(SIGTSTP, &sig_handler);
	}
	return (file_fd);
}

static int	parse_redir(t_redir *redir, t_shell *shell, int fd1, int i)
{
	int		fd2;
	char	*file;

	fd2 = 0;
	if ((shell->fd[fd1] != fd1) && (shell->fd[fd1] != -1))
		close(shell->fd[fd1]);
	if (redir->s[++i] == '&' && redir->s[i + 1] == '-')
		shell->fd[fd1] = -1;
	else if (redir->s[i] == '&' && redir->type != DREDIR &&
			redir->type != HEREDOC && ft_isdigit(redir->s[i + 1]))
		shell->fd[fd1] = dup((redir->s[i + 1] - 48));
	else
	{
		redir->s[i] == '&' ? i++ : (0);
		file = ft_strtrim(redir->s + i);
		if (is_str_quoted(file))
			file = strdup_remove_quotes(file);
		fd2 = open_file(file, redir->type, shell->fd, shell->stdin_copy);
		free(file);
		if (fd2 != -1)
			shell->fd[fd1] = fd2;
		return (fd2 == -1) ? exec_error(8, "") : 0;
	}
	return (0);
}

int			handle_redirs(t_shell *shell, t_btree *link, char **cmd, int forked)
{
	int		i;
	int		fd1;
	t_redir	*redir;

	redir = link->redir;
	while (redir)
	{
		if (!ft_isdigit(redir->s[0]))
			fd1 = ((redir->type == BREDIR) || (redir->type == HEREDOC)) ? 0 : 1;
		else
			fd1 = ft_atoi(redir->s);
		i = (redir->type == REDIR || redir->type == DREDIR) ?
		ft_strrchr_index(redir->s, '>') : ft_strrchr_index(redir->s, '<');
		if (parse_redir(redir, shell, fd1, i) == -1)
		{
			close_and_reset_fd(shell->fd);
			return (-1);
		}
		redir = redir->next;
	}
	return (forked ? redir_exec(shell, cmd) : redir_fork(cmd, shell));
}
