/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaulo-b <jpaulo-b@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 10:44:36 by jbdmc             #+#    #+#             */
/*   Updated: 2026/06/01 12:11:08 by jpaulo-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_exit(t_shell *shell)
{
	cleanup_redirections(shell);//
	if (isatty(STDIN_FILENO))
		printf(EXIT);
	exit(shell->exit_status);
}
