/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_error_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoehler <fkoehler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/29 14:58:58 by fkoehler          #+#    #+#             */
/*   Updated: 2016/11/30 17:05:29 by fkoehler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int		export_error(int errnum, char *arg)
{
	if (errnum == 0)
	{
		ft_putstr_fd("export: illegal option -- ", STDERR_FILENO);
		ft_putstr_fd(++arg, STDERR_FILENO);
	}
	ft_putstr_fd("\nusage: export [-p] [name[=value]]...\n", STDERR_FILENO);
	return (1);
}
