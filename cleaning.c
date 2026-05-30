/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 10:44:36 by jbdmc             #+#    #+#             */
/*   Updated: 2026/05/30 17:41:19 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_exit(t_shell *shell)
{
	free_all();
	if (isatty(STDIN_FILENO))
		printf(EXIT);
	exit(shell->exit_status);
}
