/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inwagner <inwagner@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 21:09:26 by inwagner          #+#    #+#             */
/*   Updated: 2023/08/26 14:18:02 by inwagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libs/libft/incl/libft.h"

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <string.h>
# include <fcntl.h>
# include <dirent.h>
# include <curses.h>
# include <termios.h>
# include <termcap.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <readline/history.h>
# include <readline/readline.h>

# define OUT_OF_MEMORY 1
# define ACTIVE 0
# define INACTIVE 1
# define DEFAULT 2

/*	REDIRECTORS
** >>	append
** <<	heredoc
** >	overwrite
** <	input
** |	pipe
*/
enum e_type
{
	APPEND = 1,
	HEREDOC,
	OVERWRITE,
	INPUT,
	PIPE,
	BUILTIN,
	EXEC,
	ARGUMENT
};

/*	Lists
*/
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_token
{
	char			*str;
	enum e_type		type;
	struct s_token	*next;
	struct s_token	*prev;
}					t_token;

/*	Structs
*/
typedef struct s_ctrl
{
	char			*input;
	struct s_token	*tokens;
	struct s_env	*env;
	char			**pbox;
	int				status;
}					t_ctrl;

/*	Functions
*/
void	prompt_user(const char *prompt);

void	set_signals(int mode);
void	quick_sort(char **strings, int low, int high);

void	update_env(char **argv, char *cmd, char *exec);
void	set_var(const char *src, t_env *node);
t_env	*parse_env(char **env);
t_env	*remove_var(char *str, t_env *list);
t_env	*search_var(char *var);
t_env	*add_var(t_env *prev, char *var);
char	*get_var_value(char *value);
char	**stringify_env(t_env *list);

t_ctrl	*get_control(void);
void	exit_program(int code);
void	clear_tokens(t_token *token);
void	clear_pbox(char **array);

char	*get_exec_path(char *env_path, char *cmd);

int		validate_input(char *input);
int		is_bracket(char c);
int		is_pipe(char c);
void	get_quote(char *input, int *i);

int		is_var(char var);
int		is_builtin(char *cmd);
int		is_redirector(char *red);
int		is_quote(char quote);

int		tokenization(char *input);

int		has_var(char *str);
void	free_pbox(char **pbox, int size);
void	null_pbox(char **pbox, int size);
char	*copy_str(char *input, int start, int len);
void	link_token(t_token *current, t_token *last);

char	*set_pipe_token(char *input, int *i, t_token *token);
char	*set_redirector_token(char *input, int *i, t_token *token);
char	*set_str_token(char *input, int *i);

char	*set_quoted_token(char *input, int *i);
char	*set_expanded_token(char *input, int *i);

char	*expand_token(char **str);
char	*get_var(char *var, int *i);

int		parser(void);

#endif