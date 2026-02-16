/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaobarb <joaobarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 11:00:00 by jbdmc             #+#    #+#             */
/*   Updated: 2026/02/12 14:53:52 by joaobarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**  helper function to check if the continuation process is interrupted by 
** a CTRL-C or CTRL-D command, properly handling both cases.
*/
static int	handle_interruptions(char *continuation, int in_single)
{
	char	quote;

	if (!continuation) // Check if readline returned NULL (EOF or interruption)
	{
		if (in_single) // Determine which quote type was unclosed
			quote = '\'';
		else
			quote = '"';
		printf("minishell: unexpected EOF while looking for matching ");
		printf("`%c'\n", quote); // Print the unclosed quote character
		printf("minishell: syntax error: unexpected end of file\n");
		return (0); // Return 0 to indicate interruption
	}
	return (1); // Return 1 if continuation is valid
}

/*
**  helper function to update quote states that also
** tracks whether we're inside single or double quotes.
*/
static void	update_quote_state(char c, int *in_single, int *in_double)
{
	if (c == '\'' && !(*in_double)) // Toggle single quote state if not inside double quotes
		*in_single = !(*in_single);
	else if (c == '"' && !(*in_single)) // Toggle double quote state if not inside single quotes
		*in_double = !(*in_double);
}

/*
**  helper function to check quote state in a string,
** updating in_single and in_double flags, and returning 1 if balanced, 0 if not.
*/
static int	check_quote_state(char *str, int *in_single, int *in_double)
{
	int	i;

	i = 0;
	*in_single = 0; // Initialize single quote flag
	*in_double = 0; // Initialize double quote flag
	while (str && str[i]) // Iterate through each character in the string
	{
		update_quote_state(str[i], in_single, in_double); // Update quote states
		i++;
	}
	return (!(*in_single) && !(*in_double)); // Return 1 if quotes are balanced
}

/*
**  wrapper function to check if quotes are balanced in a string,
** returning 1 if balanced, 0 if not balanced.
*/
char	*read_input_with_continuation(char *line)
{
	int		in_single; // Flag for single quote state
	int		in_double; // Flag for double quote state
	char	*temp; // Temporary string for joining
	char	*continuation; // Next line of input

	if (!line) // Check if initial line is NULL
		return (NULL);
	while (!check_quote_state(line, &in_single, &in_double)) // Loop while quotes are unbalanced
	{
		continuation = readline("> "); // Read continuation line with ">" prompt
		if (!handle_interruptions(continuation, in_single)) // Check for EOF or interruption
		{
			free(line); // Free the current line
			return (NULL); // Return NULL on interruption
		}
		temp = ft_strjoin(line, "\n"); // Add newline to current line
		free(line); // Free old line
		line = ft_strjoin(temp, continuation); // Join with continuation
		free(temp); // Free temporary string
		free(continuation); // Free continuation line
	}
	return (line); // Return complete line with balanced quotes
}
