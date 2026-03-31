/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaobarb <joaobarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 11:19:34 by jbdmc             #+#    #+#             */
/*   Updated: 2026/03/31 17:09:07 by joaobarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _POSIX_C_SOURCE 200809L

// Macros

# define PROMPT "minishell$ "
# define EXIT "exit\n"

// Libraries

# include "libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <signal.h>
# include <dirent.h>
# include <termio.h>
# include <term.h>

// Globals

/* 
**  Global variable used ONLY for signal handling (Ctrl + C interruption) with
** 0 = default state, 1 = interrupted by signal.
**  "extern" expands the variable to all files that include this header by declaring and assigning it the 
** initial value to the global variable directly in the header file, forcing any other C file to use that
** declaration instead of redeclaring the variable.
**  "volatile" means that the variable value can be read or modified asynchronously by something other than the
** current thread of execution (can capture and interpret changes on signals).
**  "sig_atomic_t" or "signal atomic type" is an integer type that can be accessed as an atomic entity even in the
** presence of asynchronous interrupts made by signals. This type prevents race conditions between threads and
** data corruptions by accessing the atomic value of the variable whenever a signal changes it. It basically
** automatically handles access to the variable by using thread locks per signal without the need of us to program it.
*/
extern volatile sig_atomic_t	g_signal_received;

// Structs

/*
** Environment Variables Structure that holds the content of
** all the environment variables on our program
*/
typedef struct s_env
{
	char			*name;
	char			*val;
	struct s_env	*prev;
	struct s_env	*next;
}	t_env;

/*
** Shell state structure that holds the shell's context
** int exit_status: last command exit status (for $? and error codes)
*/
typedef struct s_shell
{
	int		exit_status;
	t_env	*env;	
}	t_shell;

// More Structs

/* 
**  enumeration of types that a token can have/be
** WORD  : text (echo, cd, ls, hello, file.txt, ...)
** PIPE  : |
** GREAT : >
** DGREAT: >>
** LESS  : <
** DLESS : <<
*/
typedef enum e_tokentype
{
	WORD,
	PIPE,
	GREAT,
	DGREAT,
	LESS,
	DLESS,
}	t_tokentype;

/* 
**  char 	      *value: actual value of the token in a string
**  t_tokentype 	type: the type of the token based on the list of token types 
** (and that's why we use t_tokentype)
**  struct s_token *next: a pointer to the next node in the token struct so we 
** can "travel" inside it
*/
typedef struct s_token
{
	char			*value;		
	t_tokentype		type;
	struct s_token	*next;
}	t_token;

// Function Declaration

// cleaning.c:
void	free_all(void);
void	clean_exit(t_shell *shell);

// commands.c:
void	ft_echo(t_token **tokens, t_shell *shell);
void	ft_exit(t_token **tokens, t_shell *shell);
/* void	ft_cd(t_token **tokens, t_shell *shell);
void	ft_pwd(t_token **tokens, t_shell *shell); */
void	ft_export(t_token **tokens, t_shell *shell);
void	get_commands(t_token **tokens, t_shell *shell);

// echo.c:
void	ft_echo(t_token **tokens, t_shell *shell);

// exit.c:
void	ft_exit(t_token **tokens, t_shell *shell);

// export_helpers.c:
int		fill_node(t_env *var, char *name, char *val, t_env *cur);
void	redefine_value(t_shell *shell, char **nameval);
void	define_value(t_shell *shell, char *name, char *val);
char	**split_export_arg(char *arg);

// export.c:
char	**alpha_sort(t_shell *shell);
void	ft_export(t_token **tokens, t_shell *shell);
void	process_export_var(char **nameval, t_shell *shell);

// env.c:
void	ft_env(t_token **tokens, t_shell *shell);

// ft_lsttochpp.c:
char	**ft_lsttochpp(char **chpp, t_env *env);
char	**ft_lsttochpp_no_quotes(char **chpp, t_env *env);

// input_handling.c:
int		ft_strisspace(char *line);
char	*read_input(void);

// quote_handling.c:
int		is_quotes_balanced(char *str);
char	*read_input_with_continuation(char *line, t_shell *shell);

// parsing_helpers.c:
int		parse_pipe(char *line, size_t *i, t_token **tokens, t_shell *shell);
int		parse_less(char *line, size_t *i, t_token **tokens, t_shell *shell);
int		parse_great(char *line, size_t *i, t_token **tokens, t_shell *shell);
int		parse_single_quotes(char *line, size_t *i, t_token **tokens);
int		parse_double_quotes(char *line, size_t *i, t_token **tokens);

// parsing.c:
void	parse_input(char *line, size_t i, t_token **tokens, t_shell *shell);
int		syntaxe_error(char *line, size_t i);
int		skip_spaces(char *line, size_t *i);
void	parse_word(char *line, size_t *i, t_token **tokens);

// setup_signal_handlers.c:
void	sigint_handler(int sig);
void	setup_signal_handlers(void);

// tokenization.c:
void	add_token(char *value, t_tokentype type, t_token **tokens);

// utilities_two.c:
void	ft_swap(char **a, char **b);
char	*remove_quotes(char *nameval);
void	process_nameval_quotes(char **nameval);
int		is_valid_var_name(char *name);
void	free_nameval(char **nameval);

// utilities.c:
int		env_lstsize(t_env *env);
void	init_env(t_shell *shell, char **envp);

#endif