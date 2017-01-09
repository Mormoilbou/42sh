/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_joinf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hponcet <hponcet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/25 21:46:37 by hponcet           #+#    #+#             */
/*   Updated: 2016/04/25 23:22:45 by hponcet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include "libft.h"

static char		*join_var(char *format, char *tojoin)
{
	char buf[2];

	ft_bzero(buf, sizeof(buf));
	buf[0] = format[0];
	tojoin = ft_strjoin(tojoin, buf);
	return (tojoin);
}

static char		*join_var_free(char *str, char *tojoin)
{
	tojoin = ft_strjoin(tojoin, str);
	free(str);
	return (tojoin);
}

char			*ft_joinf(char *format, ...)
{
	int		i;
	char	*ret;
	char	*tmp;
	va_list	ap;

	i = -1;
	if (!(ret = ft_memalloc(1)))
		return (NULL);
	va_start(ap, format);
	while (format[++i] && (tmp = ret))
	{
		if (format[i] == '%' && format[i + 1] == 's' && ++i)
			ret = ft_strjoin(ret, va_arg(ap, char *));
		else if (format[i] == '%' && format[i + 1] == 'x'
				&& format[i + 2] && format[i + 2] == 's' && (i += 2))
			ret = join_var_free(va_arg(ap, char *), ret);
		else
			ret = join_var(format + i, ret);
		free(tmp);
	}
	va_end(ap);
	return (ret);
}
