/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaobarb <joaobarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 10:00:45 by joaobarb          #+#    #+#             */
/*   Updated: 2026/02/12 15:11:30 by joaobarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**  echo helper function: takes the token right after the echo command and
** checks if that token is a valid '-n' flag.
**  If it is, defines the flag boolean to 0, so that in the end of the
** echo function it will check the value of the boolean, and 0 means it is not
** supposed to print the newline, leaving without ever printing it.
*/
static int	is_valid_n_flag(char *s)
{
	int	i;

	if (!s || s[0] != '-') 	// Check if string is NULL or doesn't start with '-'
		return (0);
	i = 1;
	if (s[i] == '\0') 		// Return 0 if string is just "-"
		return (0);
	while (s[i] == 'n') 	// Skip all 'n' characters
		i++;
	return (s[i] == '\0'); 	// Return 1 if string is "-n", "-nn", etc.
}

void	ft_echo(t_token **tokens)
{
	int		flag; 			// Flag for newline (1 = print newline, 0 = don't print)
	int		first; 			// Flag to track first word (for spacing)
	t_token	*cur; 			// Current token pointer

	flag = 1; 				// Default: print newline at end
	first = 1; 				// Start with first word
	cur = (*tokens)->next; 	// Skip the "echo" command token
	while (cur && cur->type == WORD && is_valid_n_flag(cur->value)) 	// Process all -n flags
	{
		flag = 0; 			// Found -n flag, don't print newline
		cur = cur->next; 	// Move to next token
	}
	while (cur) 			// Print all remaining tokens
	{
		if (!first) 		// Add space before words (except first)
			write(1, " ", 1);
		write(1, cur->value, ft_strlen(cur->value)); 					// Write token value
		first = 0; 			// No longer first word
		cur = cur->next; 	// Move to next token
	}
	if (flag) 				// Print newline if flag is still 1
		write(1, "\n", 1);
	return ;
}
