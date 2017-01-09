/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_tools_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoehler <fkoehler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/25 16:16:34 by fkoehler          #+#    #+#             */
/*   Updated: 2016/12/11 17:25:03 by fkoehler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int		is_chr_escaped(char const *s, int i)
{
	int	backslash;

	backslash = 0;
	while (--i >= 0 && s[i] == '\\')
		backslash++;
	return (backslash % 2);
}

char	*strsub_env_var(char *s, int start)
{
	int		i;
	char	*ret;

	i = start;
	if (s[i + 1] && !ft_isspace(s[i + 1]) && s[i + 1] != '$')
	{
		i++;
		while (s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
			i++;
		ret = ft_strsub(s, (start + 1), (i - start - 1));
		return (ret);
	}
	return (NULL);
}

char	*str_replace_var(char *s, int start, int quote)
{
	size_t	len;
	char	*tmp1;
	char	*tmp2;
	char	*ret;

	if (quote == 1)
		return (s);
	ret = s;
	if ((tmp1 = strsub_env_var(s, start)))
	{
		len = ft_strlen(tmp1);
		if ((tmp2 = env_var_to_value(tmp1)))
		{
			free(tmp1);
			ret = ft_replace_str(s, start, (len + 1), tmp2);
			free(tmp2);
			free(s);
		}
		else
			free(tmp1);
	}
	return (ret);
}

int		is_str_quoted(char *s)
{
	size_t	len;

	len = ft_strlen(s);
	if (s[0] == '\'' && s[len - 1] == '\'')
		return (1);
	else if (s[0] == '"' && s[len - 1] == '"')
		return (2);
	else if (s[0] == '`' && s[len - 1] == '`')
		return (3);
	return (0);
}

char	*strdup_remove_quotes(char *s)
{
	char	*tmp;
	size_t	len;

	len = ft_strlen(s);
	tmp = ft_strsub(s, 1, len - 2);
	free(s);
	s = tmp;
	return (s);
}
