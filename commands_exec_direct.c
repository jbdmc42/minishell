/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_exec_direct.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 17:55:00 by jbdmc             #+#    #+#             */
/*   Updated: 2026/06/02 13:14:08 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>

int	exec_direct_path(char *command, t_exec_ctx *ctx)
{
	pid_t	pid;
	int		status;
	struct sigaction old_int;
	struct sigaction ignore;

	sigemptyset(&ignore.sa_mask);
	ignore.sa_flags = 0;
	ignore.sa_handler = SIG_IGN;
	sigaction(SIGINT, &ignore, &old_int);

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
		execve(command, ctx->argv, ctx->envp);
		print_exec_error(command);
		exit(126);
	}
	if (pid < 0)
		return (-1);
	status = 0;
	while (waitpid(pid, &status, 0) == -1 && errno == EINTR)
		;
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		write(STDOUT_FILENO, "\n", 1);
	sigaction(SIGINT, &old_int, NULL);
	if (WIFEXITED(status))
		ctx->shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		ctx->shell->exit_status = 128 + WTERMSIG(status);
	else
		ctx->shell->exit_status = 1;
	return (0);
}
