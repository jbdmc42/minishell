/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 11:19:34 by jbdmc             #+#    #+#             */
/*   Updated: 2026/05/30 19:00:56 by jbdmc            ###   ########.fr       */
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
# include <limits.h>

// Globals
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

typedef struct s_pipe_ctx
{
	t_token	*tokens;
	t_token	*cmd;
	t_token	*next_token;
	t_shell	*shell;
	int		num_cmds;
	int		i;
	int		pipe_fd[2];
	int		prev_read_fd;
	int		status;
	int		child_count;
	pid_t	pid;
	pid_t	last_pid;
}	t_pipe_ctx;

// Function Declaration

// main_helpers.c:
int		process_input_line(char **line, t_shell *shell);
void	execute_line(char *line, t_shell *shell, int interactive);
int		run_interactive_cycle(t_shell *shell);
int		run_noninteractive_cycle(t_shell *shell);
void	main_loop(t_shell *shell, int interactive);

// main.c:
size_t	skip_blank_prefix(char *line);
void	execute_line(char *line, t_shell *shell, int interactive);

// cleaning.c:
void	clean_exit(t_shell *shell);

// commands.c:
void	ft_echo(t_token *tokens, t_shell *shell);
void	ft_exit(t_token *tokens, t_shell *shell);
int		ft_cd(t_token *tokens, t_shell *shell);
void	ft_pwd(void);
void	ft_export(t_token *tokens, t_shell *shell);
int		ft_unset(t_token *tokens, t_shell *shell);
void	get_commands(t_token *tokens, t_shell *shell);
char	**build_argv(t_token *tokens);
char	**build_envp(t_shell *shell);
void	free_envp_array(char **envp);
int		setup_redirections(t_token **tokens, int *saved_stdin,\
				int *saved_stdout, t_shell *shell);
void	restore_redirections(int saved_stdin, int saved_stdout);
int		search_and_execute(char *command, char **argv,\
				char **envp, t_shell *shell);

// pipes_helpers.c:
int		execute_pipe_chain(t_token *tokens, t_shell *shell);

// pipes_helpers_two.c:
void	init_pipe_ctx(t_pipe_ctx *ctx, t_token *tokens, t_shell *shell);
void	setup_pipe_child(t_pipe_ctx *ctx);
int		run_pipe_stage(t_pipe_ctx *ctx);
int		handle_pipe_fork_error(t_pipe_ctx *ctx);
int		wait_pipe_children(t_pipe_ctx *ctx);

// pipes.c:
int		count_commands(t_token *tokens);
t_token	*extract_command(t_token *tokens, t_token **next_token);
int		create_pipe(int *pipe_fd);
void	close_fd(int *fd);

// echo.c:
void	ft_echo(t_token *tokens, t_shell *shell);

// exit.c:
void	ft_exit(t_token *tokens, t_shell *shell);

// export_helpers.c:
int		fill_node(t_env *var, char *name, char *val, t_env *cur);
void	redefine_value(t_shell *shell, char **nameval);
void	define_value(t_shell *shell, char *name, char *val);
char	**split_export_arg(char *arg);

// export.c:
char	**alpha_sort(t_shell *shell);
void	ft_export(t_token *tokens, t_shell *shell);
void	process_export_var(char **nameval, t_shell *shell);

// env.c:
void	ft_env(t_token *tokens, t_shell *shell);

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
// parsing_helpers_two/three.c:
char	*ft_strjoin_free(char *a, char *b);
int		is_operator_char(char c);
char	*expand_variable_in_part(char *part, t_shell *shell);
char	*extract_word_part(char *line, size_t *i, t_shell *shell);
int		append_word_part(char **token, char *part);

// redir helpers
int		create_heredoc_fd(char *delimiter);
int		perform_dup2_and_close(int fd, int target_fd, t_shell *shell);
void	unlink_token_node(t_token **tokens, t_token *node, t_token *next);

// redir_helpers_two.c:
int		heredoc_loop(int write_fd, char *delimiter);

// redir_helpers_three.c:
int		check_target_valid(t_token *target, t_shell *shell);
int		apply_redirection_stdin(t_token **tokens, t_token *redir,\
				t_token *target, t_shell *shell);
int		apply_redirection_stdout(t_token **tokens, t_token *redir,\
				t_token *target, t_shell *shell);

// commands_extra helpers
int		exec_found_command(char *full_path, char **argv, char **envp,
			t_shell *shell);
int		exec_direct_path(char *command, char **argv, char **envp,
			t_shell *shell);
int		try_exec_in_path(char *path_copy, char *command, char **argv,
			char **envp);

// parsing.c:
void	parse_input(char *line, size_t i, t_token **tokens, t_shell *shell);
int		syntaxe_error(char *line, size_t i);
int		skip_spaces(char *line, size_t *i);
void	parse_word(char *line, size_t *i, t_token **tokens, t_shell *shell);
char	*env_get_value(t_env *env, const char *name);

// setup_signal_handlers.c:
void	sigint_handler(int sig);
void	setup_signal_handlers(void);

// tokenization.c:
void	add_token(char *value, t_tokentype type, t_token **tokens);
void	print_tokens(t_token *tokens);
void	free_tokens(t_token *tokens);

// utilities_three.c:
int		env_lstsize(t_env *env);
t_env	*get_env_node(t_env *env_list, char *key);

// utilities_two.c:
void	ft_swap(char **a, char **b);
char	*remove_quotes(char *nameval);
void	process_nameval_quotes(char **nameval);
int		is_valid_var_name(char *name);
void	free_nameval(char **nameval);

// utilities.c:
void	init_env(t_shell *shell, char **envp);

#endif