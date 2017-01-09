/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkoehler <fkoehler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/23 17:07:09 by fkoehler          #+#    #+#             */
/*   Updated: 2016/12/13 13:49:22 by hponcet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_H
# define SH_H

# include <libft.h>
# include <error.h>
# include <ft_printf.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/param.h>
# include <term.h>
# include <time.h>
# include <fcntl.h>
# include <sys/ioctl.h>
# include <dirent.h>
# include <sys/stat.h>

/*
** MAILLONS ARBRE BINAIRE
*/
# define CMD 1
# define SEM 2
# define PIP 3
# define AND 4
# define OR 5

/*
** REDIRECTIONS
*/
# define REDIR 1
# define DREDIR 2
# define BREDIR 3
# define HEREDOC 4

/*
** BUFFER READ COMMANDE
*/
# define BUF_SIZE 4096

/*
** TABLES DE HASH
*/
# define HASHLEN 5000

/*
** GLOBING
*/
typedef struct			s_glob
{
	char				*name;
	char				*path;
	struct s_glob		*next;
}						t_glob;

/*
** TABLES DE HASH
*/
typedef struct			s_hash
{
	char				*name;
	char				*value;
	struct s_hash		*next;
}						t_hash;

/*
** COMPLETION
*/
typedef struct			s_compl
{
	int					type;
	size_t				len;
	size_t				id;
	char				*name;
	struct s_compl		*next;
	struct s_compl		*prev;
}						t_compl;

/*
** REDIRECTIONS
*/
typedef struct			s_redir
{
	int					type;
	char				*s;
	struct s_redir		*next;
}						t_redir;

/*
** ENVIRONNEMENT
*/
typedef struct			s_env
{
	char				*var;
	char				*val;
	int					local;
	struct s_env		*next;
}						t_env;

/*
** CARACTERES INPUT
*/
typedef struct			s_input
{
	char				c;
	struct s_input		*prev;
	struct s_input		*next;
}						t_input;

/*
** HISTORIQUE
*/
typedef	struct			s_hist
{
	unsigned int		timestamp;
	struct s_input		*input;
	struct s_hist		*prev;
	struct s_hist		*next;
}						t_hist;

/*
** ARBRE BINAIRE
*/
typedef struct			s_btree
{
	int					type;
	char				*str;
	struct s_redir		*redir;
	struct s_btree		*left;
	struct s_btree		*right;
}						t_btree;

/*
** STRUCTURE GENERALE
*/
typedef struct			s_shell
{
	int					stdin_copy;
	int					fd[4];
	int					pid;
	int					status;
	int					col;
	int					row;
	size_t				winsize;
	size_t				input_len;
	size_t				p_len;
	t_env				*env_lst;
	t_hist				*hist;
	t_hash				**hash_bin;
	int					hist_end;
	t_input				*input;
	t_input				*input_save;
	t_input				*buffer;
	t_input				*curs_pos;
	char				*input_buf;
	t_btree				*tree;
	struct termios		termios;
	struct termios		term_save;
}						t_shell;

/*
** ERREURS
*/
int						ft_put_error(char *error, int action);
int						exec_error(int errnum, char *arg);
int						cmd_error(int errnum, char c, char *s);
int						cd_error(int errnum, char *arg);
int						env_error(int errnum, int flag);
void					env_var_error(int errnum, char *cmd, char *arg);
int						export_error(int errnum, char *arg);
int						exit_error(int errnum, char *arg);

/*
** FREE
*/
void					free_input_list(t_input **input, size_t *nb_elem);
void					free_tmp_inputs(t_shell *shell, int reset_save);
void					free_env_var(t_env *env_var);
void					free_env_lst(t_env **env_lst);
void					free_redirs(t_redir **redirs);
void					free_btree(t_btree *tree);

/*
** TERMINAL + SIGNAUX
*/
void					init_shell(t_shell *shell);
void					init_term(t_shell *shell);
void					restore_term(t_shell *shell);
void					reload_term(t_shell *shell);
void					sig_handler(int signum);
void					sig_handler1(int signum);
void					sig_handler_heredoc(int signum);
void					set_sig_handler(void);

/*
** OUTILS
*/
t_shell					*get_struct(t_shell *struc);
int						putchar(int c);
int						strchr_outside_quotes(char *s, char to_find);
int						strrchr_outside_quotes(char *s, char to_find);
int						strchr_redir(t_btree *link);
int						is_chr_escaped(char const *s, int i);
int						is_str_quoted(char *s);
char					*strdup_remove_quotes(char *s);
char					**strsplit_args(char const *s);
char					**str_subsplit_arg(char const *s);
size_t					ft_list_size_index(t_input *start, t_input *end);
void					ft_lst_del(t_input **input, t_input *strt,
						t_input *end);
int						lst_is_empty(t_input *lst);
size_t					lst_len(t_input *lst);
size_t					input_part_len(t_input *start, t_input *end);
char					*lst_to_str(t_input *lst);
t_input					*lst_rchr(t_input *input, char c);
t_input					*get_last_elem(t_input *lst);
t_env					*get_last_env_elem(t_env *lst);
int						is_builtin(char *cmd);

/*
** ENVIRONNEMENT
*/
void					store_environ(t_shell *shell, char **environ);
int						store_env_var(t_env **env_lst, char *var, char *val);
int						del_env_var(t_env **env_lst, char *var, int local);
int						dup_env_lst(t_env *env_lst, t_env **env_lst_cpy);
t_env					*get_env_ptr(t_env *env_lst, char *var);
char					**env_lst_to_array(t_env *env_lst);
int						check_env_var(char *env_var, char *cmd);
void					set_shell_var(t_env *env_lst, char *arg, int local);
void					store_shell_var(t_env **env_lst, char *var,
						char *val, int local);
char					*env_var_to_value(char *var);
int						set_new_pwd(t_env *env_lst);

/*
** PROMPT
*/
char					*get_prompt(t_env *env_lst);
char					*get_special_prompt(char c);
void					print_prompt(t_shell *shell, int special_prompt);

/*
** INPUT
*/
void					read_input(t_shell *shell);
void					print_input(t_shell *shell, t_input *curs_pos,
						size_t p_len);
t_input					*ft_new_link(char c);
void					ft_input_add(t_input **curs, t_input **tmp, char c);
int						parse_input(t_shell *shell, char *buf, size_t buf_len);
void					store_input(t_shell *shell, char c);
void					insert_read_buf(t_shell *shell, char *buf, size_t len);
void					delete_input(t_input **lst, t_input *input,
						t_shell *shell, int back);
void					clear_input(t_shell *shell);

/*
** EDITION LIGNE
*/
int						parse_keys1(t_shell *shell, char *buf);
int						parse_keys2(t_shell *shell, char *buf);
int						parse_keys3(t_shell *shell, char *buf, size_t buf_len);
int						move_left(t_shell *shell);
int						move_left_word(t_shell *shell);
int						move_right(t_shell *shell);
int						move_right_word(t_shell *shell);
int						move_line_start(t_shell *shell);
int						move_line_end(t_shell *shell);
int						move_line_up(t_shell *shell);
int						move_line_down(t_shell *shell);
int						backspace(t_shell *shell);
int						del(t_shell *shell);
int						cut_eol(t_shell *shell);
int						cut_all(t_shell *shell);
int						copy_eol(t_shell *shell);
int						copy_all(t_shell *shell);
void					store_buffer(t_input **buf, char c);
int						paste_buffer(t_shell *shell);
int						history_prev(t_shell *shell);
int						history_next(t_shell *shell);
t_hist					*store_hist(t_shell *shell);
void					replace_cursor(t_shell *shell, int print, int back);
size_t					get_cursor_x_pos(t_input *input,
						t_input *pos, size_t p_len);

/*
** TRAITEMENT COMMANDE
*/
int						handle_input(t_shell *shell);
int						check_input(t_shell *shell);
int						check_btree(t_btree *link);
char					check_separators(t_input *cmd, int reverse);
char					*ft_back_q(t_shell *shell, t_input *start, char *cmd);
char					valid_input(t_input *input);
char					**parse_cmd(t_env *env_lst, t_btree *link);
int						ft_launch_cmd(t_shell *shell, t_btree *tree);
int						ft_prepare_cmd(t_shell *shell, t_btree *tree);
t_btree					*store_cmd(char *str);
char					*interpret_cmd_arg(char *cmd_arg);
int						set_local_variable(t_env *env_lst, char **cmd);
char					*remove_cmd_redir(char *cmd, t_redir *redir);
char					*str_replace_var(char *s, int start, int quote);
int						replace_backslash(char **s, int i, int quote);
int						replace_tilde(char **s, int i, int quote);
int						replace_process_value(char **s, int i, int quote);
int						handle_btree(t_shell *shell, t_btree *tree);
int						handle_cmd(t_shell *shell, t_btree *link,
						int already_forked);
pid_t					redir_fork(char **cmd, t_shell *shell);
pid_t					exec_fork(char **cmd, char **env_array, t_env *env_lst,
						t_shell *shell);
pid_t					pipe_fork_father(t_shell *shell,
						t_btree *link);
pid_t					pipe_fork_child(t_shell *shell, t_btree *link);
int						handle_redirs(t_shell *shell, t_btree *link,
						char **cmd, int forked);
int						exec_redir_cmd(t_shell *shell, char **cmd);
int						fill_heredoc(char *delimiter, int *fd);

/*
** TRAITEMENT BINAIRES
*/
int						binary_cmd(char **cmd, char **env_array,
						t_env *env_lst, t_hash **htbl);
char					*get_bin_path(char *cmd, t_env *env_lst);
int						check_bin_path(char *bin_path, char *cmd);
int						exec_bin(char *bin_path, char **argv, char **env);

/*
** FILE DESCRIPTOR + RETOUR COMMANDE
*/
int						dup_std_fd(int *fd);
void					close_and_reset_fd(int *fd);
void					set_status(int *status, char **cmd);

/*
** BUILTINS
*/
int						builtins_cmd(char **cmd, t_env *env_lst,
						t_shell *shell);
int						ft_exit(char **cmd, t_shell *shell);
int						ft_cd(char **cmd, t_env *env_lst);
int						ft_echo(char **cmd, int fd);
int						ft_env(char **cmd, t_env *env_lst, int i,
						t_shell *shell);
int						ft_setenv(char **cmd, t_env **env_lst, int flag);
int						ft_unsetenv(char **cmd, t_env **env_lst);
int						ft_export(char **cmd, t_env *env_lst);
int						ft_unset(char **cmd, t_env **env_lst);

/*
** BUILTIN READ
*/
int						ft_read(char **argv, t_env **env);
void					ft_treat_read(char **argv, char **split, t_env **env,
						int start);
int						ft_p_read_opt(char **argv, t_env **env);
int						ft_s_read_opt(char **argv, t_env **env);
int						ft_n_read_opt(char **argv, t_env **env, int nb);
int						ft_d_read_opt(char **argv, t_env **env);

/*
** /////////////////// GUS /////////////////////////////
** ////////////// BUILTIN HISTORY /////////////
** builtins/bltn_history.c
** Affiche l'historique des commande.
*/
int						ft_history(char **cmd, t_shell *shell);

/*
** builtins/bltn_history_opt.c
** Recherche dans l'historique par commande ou
** par event.
*/
int						bltn_hist_checkopt(char *cmd);
t_hist					*bltn_hist_searchstr(char *cmd, t_shell *shell,
						int opt);
t_hist					*bltn_hist_searchindex(char *cmd, t_shell *shell);

/*
** builtins/bltn_hsearch.c
** Recherche dans l'historique avec une commande
** commencant par '!' et constituee d'un event ou
** d'un chiffre.
*/
void					bltn_hsearch(t_shell *shell);
int						bltn_hsearch_key(t_shell *shell, char *buf);
int						bltn_hsearch_addchar(t_shell *shell, char *buf);
int						bltn_hsearch_ret(t_shell *shell, int c);
void					bltn_hsearch_display(t_shell *shell, t_hist *hist,
						int c);

/*
** /////////////// FILE HISTORY ///////////////
** // hist_file.c
** Lis le fichier .42_history situe dans le path $HOME
** et remplis l'historique des commande au demarage
** du shell.
** Reecrit a chaque commande le fichier .42_history
*/
int						history_error(int errnum, char *arg);
int						hist_proc(t_hist *hist, char *histpath);
int						hist_to_file(t_shell *shell, t_hist *hist);
void					input_to_hist(t_shell *shell, t_input *input, char *ts);
void					file_to_hist(t_shell *shell);
char					*hist_get_histpath(t_shell *shell);

/*
** // hist_check.c
** Check les doublons dans l'historique de commandes
*/
int						hist_checkdouble(t_shell *shell);

/*
** /////////////// INPUT TOOLS ///////////////
** // input_tools.c
** Plusieurs commandes pour l'adaptaion du 42sh.
*/
char					*input_to_char(t_input *input);
t_input					*char_to_input(char *str);
size_t					input_len(t_input *input);
void					free_input_from(t_shell *shell, t_input *input);

/*
** ////////////// COMPLETION ////////////////
** // compl_display.c
** Affichage de la completion.
** Contient une boucle while(42) qui attend certaine key
** pour la completion.
*/
int						compl_initprompt(t_shell *shell, int i);
int						compl_countfile(t_compl *print);
void					compl_display(t_shell *shell, t_compl *print,
						char *find);
/*
** // compl_key.c
** Adaptation de certaines touche speciales dans la
** boucle de completion :
** - fleche directionnelles droite et gauche
** - chaque lettres ascii
** - touche tabulation (pour faire defiler)
** - delete
** - touche esc/ctrl-d
*/
int						compl_key_dir(char *buf, t_compl **print,
						t_shell *shell, int pos);
int						compl_key(char *buf, t_compl **print, t_shell *shell,
						int pos);
/*
** // compl_str.c
** Gere l'affichage des differentes strings lors du
** defilement avec la touche tab.
*/
void					compl_addstrend(t_shell *shell, char *str, int type);
void					compl_addstr(t_shell *shell, char *str);
int						compl_retstr(t_compl *tmp, t_shell *shell, int pos,
						int i);
int						compl_retstrchar(t_compl *tmp, t_shell *shell, int pos,
						char c);
/*
** // compl_getinfo.c
** Fait une liste chainee avec les informations relative au
** path courant de la completion.
*/
char					*compl_name_wesp(char *str);
char					*compl_path_noesp(char *str);
t_compl					*compl_makefile(struct dirent *s_dir, char *path);
t_compl					*compl_makechain(char *path, t_compl *ret, char *find);
void					compl_sortchain(t_compl **list, t_compl *file);
char					*compl_getfind(char *str);

/*
** // compl.c
** compl : Fonction a appeler pour la completion qui determine
** si un binaire ou un path doit etre cherche.
** compl_delchain : free la lc du path courant ou du binaire.
*/
int						compl_wis(char *str);
void					compl_delchain(t_compl *chain);
void					compl_file(t_shell *shell, char *str);
void					compl_bin(t_shell *shell, char *str);
void					compl(t_shell *shell);

/*
** // compl_getpath.c
** Fonctions de gestion des paths. (ex: ~/42sh, /tmp,
** 42sh, ./ls, /bin/ls)
*/
int						compl_getnorm(char **ret, char **pwd);
int						compl_getpath(t_shell *shell, char **ret);
char					**compl_pathbin(t_shell *shell);

/*
** //////////////////// TABLE DE HASHAGE ///////////////
** hash.c
** Fonction de hashage simple qui aditionne les char en
** ascii puis les divise par un modulo de la taille de
** la table de hashage.
** ex:
** nmax(taille max du tableau) = 10
** tab[int nmax][t_hash*]
** "yo" = (121 + 111) % nmax = 232 % 10 = 2
** tab[2]->val = "yo"
*/
t_hash					**hash_addpath(t_hash **htbl, char *path, int nb_case);
int						hash(char *name, int nb_case);
t_hash					**hash_add(t_hash **htbl, char *name,
						char *value, int nb_case);
void					hash_delhtbl(t_hash **htbl, int nb_case);
char					*hash_search(t_hash **htbl, char *name, int nb_case);

/*
** hash_new.c
** Malloc des maillons et de la table de hashage.
*/
t_hash					**hash_newtbl(int nb_case);
t_hash					*hash_newfile(char *name, char *value);

/*
** hash_bin.c
** Converti les paths des binaires vers
** (t_hash**)shell->hash_bin
*/
t_hash					**hash_bin(t_shell *shell);

/*
** ft_glob.c
*/
int						ft_glob(char **str);
char					*ft_glob_replace(char *cmd);
t_glob					*ft_glob_makefile(struct dirent *s_dir, char *path);
char					*ft_glob_tglobtostr(t_glob *lst);
void					ft_glob_sortchain(t_glob **ret, t_glob *file);
char					*ft_glob_makestr(char *path, char *find,
						char *absolute);
int						ft_glob_compare(char *s1, char *s2);
int						ft_glob_check(char *str);
void					ft_glob_path(char **ret);
void					ft_glob_delchain(t_glob *chain);
int						ft_glob_captain_crochet(char *s1, char *s2, char c);

int						ft_cursh_check(char *str);
int						ft_cursh(char **tabl);
void					ft_cursh_compose(char **tabl);
void					ft_cursh_proc(char **str, int s, int e);
void					ft_cursh_replace(char **str);
void					ft_cursh_compose_end(char **str);

char					**ft_glob_make_pathfind(char *cmd);
void					ft_glob_addpath(t_glob **tree, char *path);
void					ft_glob_maketree(char *curpath, char **paths,
						t_glob **tree, int i);
t_glob					*ft_glob_pathtree(char *cmd);
char					*ft_putain_de_norme_de_merde(char **cmd, int i);
char					*ft_glob_makestr_return(t_glob *ret);

#endif
