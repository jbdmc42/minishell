/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_helpers_three.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 16:00:00 by jbdmc             #+#    #+#             */
/*   Updated: 2026/06/01 16:52:13 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_inherited_resources(t_pipe_ctx *ctx)
{
	if (ctx->tokens)
	{
		free_tokens(ctx->tokens);
		ctx->tokens = NULL;
	}
	/* Do not free ctx->shell->env here — child processes need access to the
	   environment to execute builtins like `env`. The shell's env is shared and
	   should be cleaned up by the parent process when appropriate. */
}

void	child_dup_fds(t_pipe_ctx *ctx)
{
	if (ctx->i < ctx->num_cmds - 1)
		close_fd(&ctx->pipe_fd[0]);
	if (ctx->prev_read_fd >= 0)
	{
		dup2(ctx->prev_read_fd, STDIN_FILENO);
		close_fd(&ctx->prev_read_fd);
	}
	if (ctx->i < ctx->num_cmds - 1)
	{
		dup2(ctx->pipe_fd[1], STDOUT_FILENO);
		close_fd(&ctx->pipe_fd[1]);
	}
}

void	child_finish(t_pipe_ctx *ctx)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	free_inherited_resources(ctx);
	get_commands(ctx->cmd, ctx->shell);
	exit(ctx->shell->exit_status);
}

int	create_pipe_if_needed(t_pipe_ctx *ctx)
{
	if (ctx->i < ctx->num_cmds - 1)
	{
		if (create_pipe(ctx->pipe_fd) == -1)
		{
			close_fd(&ctx->prev_read_fd);
			if (ctx->cmd)
			{
				free_tokens(ctx->cmd);
				ctx->cmd = NULL;
			}
			return (1);
		}
	}
	return (0);
}

void	post_parent_housekeeping(t_pipe_ctx *ctx)
{
	if (ctx->pid != 0 && ctx->cmd)
	{
		free_tokens(ctx->cmd);
		ctx->cmd = NULL;
	}
	if (ctx->i == ctx->num_cmds - 1)
		ctx->last_pid = ctx->pid;
	if (ctx->prev_read_fd >= 0)
		close_fd(&ctx->prev_read_fd);
	if (ctx->i < ctx->num_cmds - 1)
	{
		close_fd(&ctx->pipe_fd[1]);
		ctx->prev_read_fd = ctx->pipe_fd[0];
	}
}
