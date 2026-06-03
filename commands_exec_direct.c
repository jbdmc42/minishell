/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_exec_direct.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 17:55:00 by jbdmc             #+#    #+#             */
/*   Updated: 2026/06/01 16:05:16 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>

int	exec_direct_path(char *command, t_exec_ctx *ctx)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execve(command, ctx->argv, ctx->envp);
		print_exec_error(command);
		exit(126);
	}
	if (pid < 0)
		return (-1);
	status = 0;
	while (waitpid(pid, &status, 0) == -1 && errno == EINTR)
		;
	if (WIFEXITED(status))
		ctx->shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		ctx->shell->exit_status = 128 + WTERMSIG(status);
	else
		ctx->shell->exit_status = 1;
	return (0);
}
