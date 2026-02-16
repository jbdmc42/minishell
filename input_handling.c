/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaobarb <joaobarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 10:44:02 by jbdmc             #+#    #+#             */
/*   Updated: 2026/02/12 14:38:44 by joaobarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**  read_input function helper: checks for an empty line and returns a boolean.
*/
int	ft_strisspace(char *line)
{
	size_t	i; // Index for iterating through line

	i = 0; // Initialize index
	if (!line) // Check if line is NULL
		return (1); // Return 1 (true) for NULL line
	while (line[i] == ' ' || line[i] == '\t') // Skip spaces and tabs
		i++; // Move to next character
	return (line[i] == '\0'); // Return 1 if only whitespace, 0 otherwise
}

/*
**  reads the line and checks for initial spaces to skip, and empty line or 
** an EOF.
**  In the end, returns the line from the prompt.
*/
char	*read_input(void)
{
	size_t	i; 													// Index for skipping spaces
	char	*line; 												// Input line from readline

	i = 0; 														// Initialize index
	line = readline(PROMPT); 									// Read input from user
	if (!line) 													// Check for EOF (Ctrl+D)
		return (NULL); 											// Return NULL on EOF
	while (line[i] && (line[i] == ' ' || line[i] == '\t')) 		// Skip leading whitespace
		i++; 													// Move to next character
	if (!ft_strisspace(line)) 									// Check if line is not only whitespace
		add_history(line); 										// Add to readline history
	return (line); 												// Return input line
}
