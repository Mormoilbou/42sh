# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fkoehler <fkoehler@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/04/26 11:46:03 by fkoehler          #+#    #+#              #
#*   Updated: 2016/12/14 16:50:32 by hponcet          ###   ########.fr       *#
#                                                                              #
# **************************************************************************** #

NAME = 42sh

FLAGS = -Wall -Werror -Wextra

vpath %.c ./src ./src/builtins

SRC = buffer.c \
	  builtins_error.c \
	  builtins_error_2.c \
	  builtins_check.c \
	  bltn_history.c \
	  bltn_history_opt.c \
	  bltn_hsearch.c \
	  cd.c \
	  check_cmd_separators.c \
	  check_redir.c \
	  cmd_check_input.c \
	  cmd_handling.c \
	  cmd_interpreting.c \
	  cmd_parsing.c \
	  cmd_storage.c \
	  compl.c \
	  compl_display.c \
	  compl_getinfo.c \
	  compl_getpath.c \
	  compl_getpath2.c \
	  compl_key.c \
	  compl_str.c \
	  cursor.c \
	  echo.c \
	  env.c \
	  env_tools.c \
	  environ.c \
	  exec_bin.c \
	  exit.c \
	  exit_status.c \
	  export.c \
	  fd_handling.c \
	  forking.c \
	  free.c \
	  free_btree.c \
	  ft_cursh.c \
	  ft_cursh_replace.c \
	  ft_cursh_compose.c \
	  ft_glob.c \
	  ft_glob_path_globed.c \
	  ft_glob_compare.c \
	  ft_glob_make.c \
	  ft_glob_path.c \
	  ft_glob_sortchain.c \
	  hash.c \
	  hash_bin.c \
	  hash_new.c \
	  heredoc.c \
	  hist_storage.c \
	  hist_file.c \
	  hist_check.c \
	  history.c \
	  input_cut_copy.c \
	  input_delete_keys.c \
	  input_handling.c \
	  input_tools.c \
	  keys_parsing.c \
	  line_moves_1.c \
	  line_moves_2.c \
	  lst_tools.c \
	  lst_tools_2.c \
	  main.c \
	  main_parsing.c \
	  print.c \
	  prompt.c \
	  putchar.c \
	  redirection.c \
	  setenv.c \
	  signals.c \
	  special_char.c \
	  str_tools.c \
	  str_tools_2.c \
	  strsplit_args.c \
	  str_subsplit_arg.c \
	  term_settings.c \
	  unset.c \
	  unsetenv.c \
	  ft_back_quote.c	\
	  ft_tools_back_quote.c	\
	  ft_read.c	\
	  ft_read_options.c	\
	  ft_error.c

# FILES
OBJS	= $(SRC:.c=.o)
O2		= $(addprefix $(OPATH), $(OBJS))

# DIRECTORIES
LIBFT	= ./libft/
OPATH	= ./obj/
INC		= ./includes/
LIBINC	= $(LIBFT)$(INC)
LIB		= $(LIBFT)libft.a

# PROCESS

$(NAME): $(LIB) $(O2)
	@gcc $(FLAGS) $(O2) -L$(LIBFT) -lft -ltermcap -I$(INC) -o $@
	@echo "\033[0;32m42sh compilation done !\033[0;m"

all: $(NAME)

$(LIB):
	@echo "\033[0;32mWaiting, libft is in compilation...\033[0;m"
	@make -C $(LIBFT)

$(OPATH)%.o: %.c
	@gcc $(FLAGS) -I $(INC) -I $(LIBINC) -o $@ -c $<

clean:
	@rm -f $(O2)
	@echo "\033[0;32mObject files deleted !\033[0;m"

fclean: clean
	@rm -f $(NAME)
	@echo "\033[0;32mExecutable deleted !\033[0;m"
	-@make fclean -C $(LIBFT)
	@echo "\033[0;32mLibft cleaned.\033[0;m"

re: fclean all
