/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_helpers_two.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 17:49:50 by jbdmc             #+#    #+#             */
/*   Updated: 2026/06/01 16:47:02 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>

int	handle_pipe_fork_error(t_pipe_ctx *ctx)
{
	perror("fork");
	close_fd(&ctx->prev_read_fd);
	if (ctx->i < ctx->num_cmds - 1)
	{
		close_fd(&ctx->pipe_fd[0]);
		close_fd(&ctx->pipe_fd[1]);
	}
	return (1);
}

int	wait_pipe_children(t_pipe_ctx *ctx)
{
	ctx->child_count = ctx->num_cmds;
	ctx->shell->exit_status = 0;
	int	last_status = 0;
	while (ctx->child_count > 0)
	{
		ctx->pid = waitpid(-1, &ctx->status, 0);
		if (ctx->pid == -1 && errno == EINTR)
			continue ;
		if (ctx->pid == ctx->last_pid)
		{
			if (WIFEXITED(ctx->status))
				ctx->shell->exit_status = WEXITSTATUS(ctx->status);
			else if (WIFSIGNALED(ctx->status))
				ctx->shell->exit_status = 128 + WTERMSIG(ctx->status);
			else
				ctx->shell->exit_status = 1;
		}
		if (WIFEXITED(ctx->status))
			last_status = WEXITSTATUS(ctx->status);
		else if (WIFSIGNALED(ctx->status))
			last_status = 128 + WTERMSIG(ctx->status);
		else
			last_status = 1;
		ctx->child_count--;
	}
	if (ctx->last_pid <= 0)
		ctx->shell->exit_status = last_status;
	return (ctx->shell->exit_status);
}
