/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 11:19:34 by jbdmc             #+#    #+#             */
/*   Updated: 2026/02/19 09:09:19 by jbdmc            ###   ########.fr       */
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
**  "volatile" means that the variable value can be read or modified asynchronously by something other than the the
** current thread of execution (can capture and interpret changes on signals).
**  "sig_atomic_t" or "signal atomic type" is an integer type that can be accessed as an atomic entity even in the
** presence of of asynchronous interrupts made by signals. This type prevents race conditions between threads and
** data corruptions by accessing the atomic value of the variable whenever a signal changes it. It basically
** automatically handles access to the variable by using thread locks per signal without the need of us to program it.
*/
extern volatile sig_atomic_t	g_signal_received;

// Structs

/*
** Shell state structure that holds the shell's context
** int exit_status: last command exit status (for $? and error codes)
*/
typedef struct s_shell
{
	int			exit_status;
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
	DLESS
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
void	ft_pwd(t_token **tokens, t_shell *shell);
void	ft_export(t_token **tokens, t_shell *shell); */
void	get_commands(t_token **tokens, t_shell *shell);

// echo.c:
void	ft_echo(t_token **tokens, t_shell *shell);

// exit.c:
void	ft_exit(t_token **tokens, t_shell *shell);

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

// setup_signal_handlers.c:
void	sigint_handler(int sig);
void	setup_signal_handlers(void);

// tokenization.c:
void	add_token(char *value, t_tokentype type, t_token **tokens);

// temp
void	print_tokens(t_token *tokens);

#endif