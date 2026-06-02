/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 15:40:00 by jbdmc             #+#    #+#             */
/*   Updated: 2026/06/02 12:09:36 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <string.h>
#include <unistd.h>

static int	should_expand_heredoc(const char *delimiter)
{
	if (!delimiter || !*delimiter)
		return (1);
	if (*delimiter == '\x01')
		return (0);
	return (1);
}

static void	cleanup_dless_nodes(t_token **tokens, t_token *redir,
	t_token *target)
{
	t_token	*next;

	next = target->next;
	unlink_token_node(tokens, redir, next);
	free(target->value);
	free(target);
	free(redir->value);
	free(redir);
}

static int	apply_input_redirection(t_token **tokens, t_token *redir,
		t_token *target, t_shell *shell)
{
	int	fd;

	if (redir->type == DLESS)
	{
		if (target->heredoc_fd >= 0)
		{
			fd = target->heredoc_fd;
			/* mark consumed in this token copy so free doesn't double-close */
			target->heredoc_fd = -1;
		}
		else
			fd = create_heredoc_fd(target->value,
				should_expand_heredoc(target->value), shell);
	}
	else
		fd = open(target->value, O_RDONLY);
	if (fd < 0)
	{
		if (redir->type == DLESS)
			return (-1);
		printf("%s: %s: %s\n", NAME, target->value, strerror(errno));
		shell->exit_status = 1;
		return (-1);
	}
	cleanup_dless_nodes(tokens, redir, target);
	return (fd);
}

int	apply_redir_by_type(t_token **tokens, t_token *redir,
			t_token *target, t_shell *shell)
{
	if (redir->type == LESS || redir->type == DLESS)
		return (apply_redirection_stdin(tokens, redir, target, shell));
	else
		return (apply_redirection_stdout(tokens, redir, target, shell));
}

int	handle_one_redirection(t_token **tokens, t_token *redir,
			t_shell *shell)
{
	t_token	*target;

	if (!tokens || !redir)
		return (-1);
	target = redir->next;
	if (check_target_valid(target, shell) == -1)
		return (-1);
	if (redir->type == DLESS || redir->type == LESS)
		return (apply_input_redirection(tokens, redir, target, shell));
	return (apply_redir_by_type(tokens, redir, target, shell));
}

int	process_token_redirections(t_token **tokens, t_shell *shell,
			int saved_stdin, int saved_stdout)
{
	t_token	*current;
	t_token	*target;
	t_token	*next;
	int		pending_stdin_fd;
	int		fd;

	if (!tokens || !*tokens)
		return (0);
	pending_stdin_fd = -1;
	current = *tokens;
	while (current)
	{
		if (current->type == LESS || current->type == DLESS
			|| current->type == GREAT || current->type == DGREAT)
		{
			target = current->next;
			next = (target) ? target->next : NULL;
			if (check_target_valid(target, shell) == -1)
				return (restore_redirections(saved_stdin, saved_stdout), -1);
			if (current->type == LESS || current->type == DLESS)
			{
				if (pending_stdin_fd >= 0)
					close(pending_stdin_fd);
				fd = apply_input_redirection(tokens, current, target, shell);
				if (fd < 0)
					return (restore_redirections(saved_stdin, saved_stdout), -1);
				pending_stdin_fd = fd;
			}
			else if (apply_redirection_stdout(tokens, current, target, shell) == -1)
				return (restore_redirections(saved_stdin, saved_stdout), -1);
			current = next;
			continue ;
		}
		current = current->next;
	}
	if (pending_stdin_fd >= 0)
	{
		if (perform_dup2_and_close(pending_stdin_fd, STDIN_FILENO, shell) == -1)
			return (restore_redirections(saved_stdin, saved_stdout), -1);
	}
	return (0);
}
