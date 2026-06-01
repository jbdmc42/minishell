/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 17:49:50 by jbdmc             #+#    #+#             */
/*   Updated: 2026/06/01 15:14:06 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_inherited_resources(t_pipe_ctx *ctx);
void	child_dup_fds(t_pipe_ctx *ctx);
void	child_finish(t_pipe_ctx *ctx);
int		create_pipe_if_needed(t_pipe_ctx *ctx);
void	post_parent_housekeeping(t_pipe_ctx *ctx);

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
	child_dup_fds(ctx);
	child_finish(ctx);
}

static int	fork_and_handle(t_pipe_ctx *ctx)
{
	ctx->pid = fork();
	if (ctx->pid == -1)
	{
		if (ctx->cmd)
		{
			free_tokens(ctx->cmd);
			ctx->cmd = NULL;
		}
		return (handle_pipe_fork_error(ctx));
	}
	if (ctx->pid == 0)
		setup_pipe_child(ctx);
	return (0);
}

int	run_pipe_stage(t_pipe_ctx *ctx)
{
	ctx->cmd = extract_command(ctx->tokens, &ctx->next_token);
	if (create_pipe_if_needed(ctx) == 1)
		return (1);
	if (fork_and_handle(ctx) == 1)
		return (1);
	post_parent_housekeeping(ctx);
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
