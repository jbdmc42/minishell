/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 17:49:50 by jbdmc             #+#    #+#             */
/*   Updated: 2026/05/30 17:58:30 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_pipe_ctx(t_pipe_ctx *ctx, t_token *tokens, t_shell *shell)
{
	ctx->tokens = tokens;
	ctx->cmd = NULL;
	ctx->next_token = NULL;
	ctx->shell = shell;
	ctx->num_cmds = count_commands(tokens);
	ctx->i = 0;
	ctx->pipe_fd[0] = -1;
	ctx->pipe_fd[1] = -1;
	ctx->prev_read_fd = -1;
	ctx->status = 0;
	ctx->child_count = 0;
	ctx->pid = -1;
	ctx->last_pid = -1;
}

void	setup_pipe_child(t_pipe_ctx *ctx)
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
	get_commands(ctx->cmd, ctx->shell);
	exit(ctx->shell->exit_status);
}

int	run_pipe_stage(t_pipe_ctx *ctx)
{
	ctx->cmd = extract_command(ctx->tokens, &ctx->next_token);
	if (ctx->i < ctx->num_cmds - 1)
	{
		if (create_pipe(ctx->pipe_fd) == -1)
		{
			close_fd(&ctx->prev_read_fd);
			return (1);
		}
	}
	ctx->pid = fork();
	if (ctx->pid == -1)
		return (handle_pipe_fork_error(ctx));
	if (ctx->pid == 0)
		setup_pipe_child(ctx);
	if (ctx->i == ctx->num_cmds - 1)
		ctx->last_pid = ctx->pid;
	if (ctx->prev_read_fd >= 0)
		close_fd(&ctx->prev_read_fd);
	if (ctx->i < ctx->num_cmds - 1)
	{
		close_fd(&ctx->pipe_fd[1]);
		ctx->prev_read_fd = ctx->pipe_fd[0];
	}
	return (0);
}

int	execute_pipe_chain(t_token *tokens, t_shell *shell)
{
	t_pipe_ctx	ctx;

	init_pipe_ctx(&ctx, tokens, shell);
	while (ctx.i < ctx.num_cmds)
	{
		if (run_pipe_stage(&ctx) == 1)
			return (1);
		ctx.tokens = ctx.next_token;
		ctx.i++;
	}
	return (wait_pipe_children(&ctx));
}
