/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: MrRobot <mimazouz@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/17 12:07:03 by MrRobot           #+#    #+#             */
/*   Updated: 2016/12/12 12:46:51 by hponcet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# define ERR_PATH "42sh: Unable to find the $PATH variable\n"
# define ER_OFD "42sh: unable to open the terminal device file\n"
# define ER_GETTERM "42sh: unable to get the terminal type\n"
# define ER_GETENT "42sh: unable to find the terminfo database\n"
# define ER_GETATTR "42sh: unable to retrieve the terminal parameters\n"
# define ER_SETATTR "tcsetattr: unable to set the terminal parameters\n"
# define ER_MEM "malloc: memory allocation has failed\n"
# define ER_READ "read: an error occured while reading the input\n"
# define ER_TERM_C "42sh: terminal capabilities not supported\n"
# define ER_IOCTL "ioctl: unable to get the terminal state infos\n"
# define ER_OPT "42sh: Invalid option\n"
# define ER_ARG "42sh: Option requires an argument\n"
# define ER_PATH "environment variable PATH not set or empty\n"

#endif
