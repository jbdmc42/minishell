/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 17:49:50 by jbdmc             #+#    #+#             */
/*   Updated: 2026/06/02 13:14:08 by jbdmc            ###   ########.fr       */
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

static int	pre_create_heredocs(t_pipe_ctx *ctx)
{
	t_token *cur;
	t_token *target;
	int     should_expand;
	int     fd;

	cur = ctx->tokens;
	while (cur)
	{
		if (cur->type == DLESS)
		{
			target = cur->next;
			if (!target)
				return (0);
			if (target->heredoc_fd >= 0)
			{
				cur = cur->next;
				continue ;
			}
			should_expand = 1;
			if (target->value && target->value[0] == '\x01')
				should_expand = 0;
			fd = create_heredoc_fd(target->value, should_expand, ctx->shell);
			if (fd < 0)
			{
				/* cleanup any previously created fds */
				cur = ctx->tokens;
				while (cur)
				{
					if (cur->heredoc_fd >= 0)
					{
						close(cur->heredoc_fd);
						cur->heredoc_fd = -1;
					}
					cur = cur->next;
				}
				return (1);
			}
			target->heredoc_fd = fd;
		}
		cur = cur->next;
	}
	return (0);
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
	int	status;
	struct sigaction old_int;
	struct sigaction ignore;

	init_pipe_ctx(&ctx, tokens, shell);
	ctx.tokens = tokens;
	/* Parent should ignore SIGINT while creating/forking pipeline children
	   so Ctrl+C only affects the child processes. */
	sigemptyset(&ignore.sa_mask);
	ignore.sa_flags = 0;
	ignore.sa_handler = SIG_IGN;
	sigaction(SIGINT, &ignore, &old_int);

	/* Pre-create all heredoc fds for the whole pipeline to avoid deadlock */
	if (pre_create_heredocs(&ctx) == 1)
	{
		sigaction(SIGINT, &old_int, NULL);
		return (1);
	}
	while (ctx.i < ctx.num_cmds)
	{
		if (run_pipe_stage(&ctx) == 1)
			return (1);
		ctx.tokens = ctx.next_token;
		ctx.i++;
	}

	/* Close parent's copies of precreated heredoc fds; children inherited theirs */
	{
		t_token *cur = tokens;
		while (cur)
		{
			if (cur->heredoc_fd >= 0)
			{
				close(cur->heredoc_fd);
				cur->heredoc_fd = -1;
			}
			cur = cur->next;
		}
	}
	status = wait_pipe_children(&ctx);
	sigaction(SIGINT, &old_int, NULL);
	return (status);
}
