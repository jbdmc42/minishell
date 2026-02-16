/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaobarb <joaobarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 11:19:24 by jbdmc             #+#    #+#             */
/*   Updated: 2026/02/12 14:37:48 by joaobarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status;

/*
**  helper function that checks for an empty input line, returning the stopping 
** point without actually moving in the line position on the main loop.
*/
static size_t	treat_empty_input(char *line, size_t i)
{
	while (line[i] && (line[i] == ' ' || line[i] == '\t')) // Skip spaces and tabs
		i++; // Move to next character
	return (i); // Return position after whitespace
}

/*
**  main loop of the program: acts as a listener that receives each 
** line sent by the user and then, after checking if the line is valid
** (not empty, not only a '\0'), sends it to parser that will then divide it 
** into separate tokens that we can interpret easier.
*/
static void	main_loop(void)
{
	char	*line; // Input line from readline
	size_t	i; // Index for parsing
	t_token	*tokens; // Token list

	while (1) // Infinite loop for shell prompt
	{
		i = 0; // Reset index
		line = readline(PROMPT); // Read input from user
		if (!line) // Check for EOF (Ctrl+D)
			clean_exit(); // Exit cleanly
		if (line[treat_empty_input(line, i)] == '\0') // Check if line is empty or only whitespace
		{
			free(line); // Free empty line
			continue ; // Skip to next iteration
		}
		add_history(line); // Add line to readline history
		line = read_input_with_continuation(line); // Handle unclosed quotes with continuation
		if (!line) // Check if continuation was interrupted
			continue ; // Skip to next iteration
		tokens = NULL; // Initialize token list
		parse_input(line, i, &tokens); // Parse line into tokens
		free(line); // Free input line
	}
}

/*
**  main program function: starts the user input request loop and prepares the
** signal handlers for CTRL-D, CTRL-C and CTRL-\.
*/
int	main(void)
{
	g_exit_status = 0; // Initialize exit status to 0
	setup_signal_handlers(); // Setup signal handlers for SIGINT and SIGQUIT
	main_loop(); // Start the main shell loop
	return (g_exit_status); // Return exit status (never reached)
}
