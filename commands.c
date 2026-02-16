/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaobarb <joaobarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 14:18:09 by jbdmc             #+#    #+#             */
/*   Updated: 2026/02/12 15:05:48 by joaobarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_commands(t_token **tokens)
{
	if (!tokens || !*tokens || !(*tokens)->value) 				// Check if tokens are NULL or empty
		return ; 												// Return if no tokens to process
	if (ft_strcmp((*tokens)->value, "echo") == 0) 				// Check if command is "echo"
		ft_echo(tokens); 										// Execute echo command
	else if (ft_strcmp((*tokens)->value, "exit") == 0) 			// Check if command is "exit"
		ft_exit(tokens); 										// Execute exit command
	else 														// Command not recognized
	{
		printf("%s: command not found\n", (*tokens)->value); 	// Print error message
		g_exit_status = 127; 									// Set exit status to 127 (command not found)
	}
}
