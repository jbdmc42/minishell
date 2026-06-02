/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_signal_handlers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 13:27:38 by jbdmc             #+#    #+#             */
/*   Updated: 2026/06/02 13:14:08 by jbdmc            ###   ########.fr       */
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
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_signal_handlers(int interactive)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (interactive)
	{
		sa.sa_handler = sigint_handler;
		sigaction(SIGINT, &sa, NULL);
		signal(SIGQUIT, SIG_IGN);
	}
	else
	{
		sa.sa_handler = SIG_DFL;
		sigaction(SIGINT, &sa, NULL);
		signal(SIGQUIT, SIG_IGN);
	}
}
