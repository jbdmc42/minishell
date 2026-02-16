/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaobarb <joaobarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 09:15:11 by joaobarb          #+#    #+#             */
/*   Updated: 2026/02/12 14:37:48 by joaobarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
**  exit helper function: in short, an atoi function that returns a boolean 
** instead of a converted number. 
**  Checks if the token received if numeric or not.
*/
static int	is_numeric(char *str)
{
	int	i;

	if (!str || !*str) // Check if string is NULL or empty
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-') // Skip optional sign
		i++;
	if (!str[i]) // Return 0 if string is only a sign
		return (0);
	while (str[i]) // Check if all remaining characters are digits
	{
		if (!ft_isdigit(str[i])) // Return 0 if non-digit found
			return (0);
		i++;
	}
	return (1); // Return 1 if string is numeric
}

/*
**  exit helper function: counts the number of tokens to check if there are 
** more than 2.
*/
static int	count_tokens(t_token *tokens)
{
	int		count; // Token counter
	t_token	*cur; // Current token pointer

	count = 0; // Initialize counter
	cur = tokens; // Start at first token
	while (cur) // Iterate through all tokens
	{
		count++; // Increment counter
		cur = cur->next; // Move to next token
	}
	return (count); // Return total count
}

void	ft_exit(t_token **tokens)
{
	int		token_count; // Number of tokens

	printf("exit\n"); // Print "exit" message
	token_count = count_tokens(*tokens); // Count tokens (command + arguments)
	if (token_count > 2) // Check if too many arguments
	{
		printf("minishell: exit: too many arguments\n");
		g_exit_status = 1; // Set exit status to 1
		return ; // Return without exiting
	}
	if (token_count == 2) // Check if exit code argument provided
	{
		if (!is_numeric((*tokens)->next->value)) // Check if argument is numeric
		{
			printf("minishell: exit: %s: numeric argument required\n",
				(*tokens)->next->value);
			g_exit_status = 2; // Set exit status to 2 for invalid argument
			free_all(); // Free all resources
			exit(g_exit_status); // Exit with error status
		}
		g_exit_status = ft_atoi((*tokens)->next->value); // Convert argument to exit code
	}
	free_all(); // Free all resources
	exit(g_exit_status); // Exit with specified or default status
}
