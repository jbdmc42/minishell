/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_signal_handlers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaobarb <joaobarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 13:27:38 by jbdmc             #+#    #+#             */
/*   Updated: 2026/02/12 14:37:48 by joaobarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** [SIGINT Handler / Ctrl + C Handler] 
** as we don't want our process to close,
** the handler will only interrupt the
** current line, create a new one, and
** redisplay the prompt.
*/

void	sigint_handler(int sig)
{
	(void)sig; // Unused parameter
	write(STDOUT_FILENO, "\n", 1); // Print newline
	rl_on_new_line(); // Tell readline we're on a new line
	rl_replace_line("", 0); // Clear the current line
	rl_redisplay(); // Redisplay the prompt
	g_exit_status = 130; // Set exit status to 130 (128 + SIGINT)
}

void	setup_signal_handlers(void)
{
	struct sigaction	sa; // Signal action structure

	sigemptyset(&sa.sa_mask); // Initialize signal mask to empty
	sa.sa_flags = 0; // No special flags
	sa.sa_handler = sigint_handler; // Set handler function for SIGINT
	sigaction(SIGINT, &sa, NULL); // Register SIGINT handler
	signal(SIGQUIT, SIG_IGN); // Ignore SIGQUIT (Ctrl+\)
}
