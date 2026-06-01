/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 10:44:36 by jbdmc             #+#    #+#             */
/*   Updated: 2026/06/01 14:25:14 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_exit(t_shell *shell)
{
	if (!shell)
		exit(0);
	cleanup_redirections(shell);
	free_shell_env(shell);
	if (isatty(STDIN_FILENO))
		printf(EXIT);
	exit(shell->exit_status);
}
