/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaobarb <joaobarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 11:19:34 by jbdmc             #+#    #+#             */
/*   Updated: 2026/02/12 14:12:05 by joaobarb         ###   ########.fr       */
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
** "extern" is used so that the variable is actually defined on the header file, 
** and not only declared and redefined in each .c file with this header present 
*/
extern int	g_exit_status;

// Structs

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
void	clean_exit(void);

// commands.c:
void	ft_echo(t_token **tokens);
void	ft_exit(t_token **tokens);
/* void	ft_cd(t_token **tokens);
void	ft_pwd(t_token **tokens);
void	ft_export(t_token **tokens); */
void	get_commands(t_token **tokens);

// echo.c:
void	ft_echo(t_token **tokens);

// exit.c:
void	ft_exit(t_token **tokens);

// input_handling.c:
int		ft_strisspace(char *line);
char	*read_input(void);

// quote_handling.c:
int		is_quotes_balanced(char *str);
char	*read_input_with_continuation(char *line);

// parsing_helpers.c:
int		parse_pipe(char *line, size_t *i, t_token **tokens);
int		parse_less(char *line, size_t *i, t_token **tokens);
int		parse_great(char *line, size_t *i, t_token **tokens);
int		parse_single_quotes(char *line, size_t *i, t_token **tokens);
int		parse_double_quotes(char *line, size_t *i, t_token **tokens);

// parsing.c:
void	parse_input(char *line, size_t i, t_token **tokens);
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