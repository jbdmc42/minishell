/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_signal_handlers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaobarb <joaobarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 13:27:38 by jbdmc             #+#    #+#             */
/*   Updated: 2026/02/16 11:43:01 by joaobarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** [SIGINT Handler / Ctrl + C Handler] 
** Interrupt current input and return to prompt.
*/

void	sigint_handler(int sig)
{
	(void)sig;
	g_signal_received = 1;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_signal_handlers(void)
{
	struct sigaction	sa; 									// Signal action structure

	sigemptyset(&sa.sa_mask); 									// Initialize signal mask to empty
	sa.sa_flags = 0; 											// No special flags
	sa.sa_handler = sigint_handler; 							// Set handler function for SIGINT
	sigaction(SIGINT, &sa, NULL); 								// Register SIGINT handler
	signal(SIGQUIT, SIG_IGN); 									// Ignore SIGQUIT (Ctrl+\)
}
