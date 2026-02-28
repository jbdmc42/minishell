/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaobarb <joaobarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 11:19:24 by jbdmc             #+#    #+#             */
/*   Updated: 2026/02/24 16:43:35 by joaobarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

/*
**  Helper function that checks for an empty input line.
*/
static size_t	treat_empty_input(char *line, size_t i)
{
	while (line[i] && (line[i] == ' ' || line[i] == '\t'))
		i++;
	return (i);
}

/*
**  Handle readline input and check for interrupts or empty lines.
**  Returns 1 if processing should continue, 0 if should skip to next iteration.
*/
static int	process_input_line(char **line, t_shell *shell)
{
	if (!*line)
		clean_exit(shell);
	if (g_signal_received)
	{
		shell->exit_status = 130;
		return (0);
	}
	if ((*line)[treat_empty_input(*line, 0)] == '\0')
	{
		free(*line);
		return (0);
	}
	return (1);
}

/*
**  Process and execute the validated input line.
*/
static void	execute_line(char *line, t_shell *shell)
{
	t_token	*tokens;
	char	*validated_line;

	validated_line = read_input_with_continuation(line, shell);
	if (!validated_line)
	{
		free(line);
		return ;
	}
	add_history(line);
	tokens = NULL;
	parse_input(validated_line, 0, &tokens, shell);
	free(validated_line);
}

/*
**  Main loop: reads and processes user input continuously.
*/
static void	main_loop(t_shell *shell)
{
	char	*line;

	while (1)
	{
		g_signal_received = 0;
		line = readline(PROMPT);
		if (!process_input_line(&line, shell))
			continue ;
		execute_line(line, shell);
	}
}

/*
**  main program function: starts the user input request loop and prepares the
** signal handlers for CTRL-D, CTRL-C and CTRL-\.
*/
int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	t_shell	shell; 												// Shell state structure

	shell.exit_status = 0; 										// Initialize exit status to 0
	init_env(&shell, envp);										// Initialize environment variables from envp
	setup_signal_handlers(); 									// Setup signal handlers for SIGINT and SIGQUIT
	main_loop(&shell); 											// Start the main shell loop
	return (shell.exit_status); 								// Return exit status (never reached)
}
