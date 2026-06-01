/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 15:40:00 by jbdmc             #+#    #+#             */
/*   Updated: 2026/06/01 16:29:50 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

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
	int		fd;

	if (!tokens || !redir)
		return (-1);
	target = redir->next;
	if (check_target_valid(target, shell) == -1)
		return (-1);
	if (redir->type == DLESS)
	{
		fd = create_heredoc_fd(target->value);
		if (fd < 0)
			return (-1);
		if (perform_dup2_and_close(fd, STDIN_FILENO, shell) == -1)
			return (-1);
		cleanup_dless_nodes(tokens, redir, target);
		return (0);
	}
	return (apply_redir_by_type(tokens, redir, target, shell));
}

int	process_token_redirections(t_token **tokens, t_shell *shell,
			int saved_stdin, int saved_stdout)
{
	t_token	*current;
	t_token	*prev;

	if (!tokens || !*tokens)
		return (0);
	prev = NULL;
	current = *tokens;
	while (current)
	{
		if (current->type == LESS || current->type == DLESS
			|| current->type == GREAT || current->type == DGREAT)
		{
			if (handle_one_redirection(tokens, current, shell) == -1)
				return (restore_redirections(saved_stdin, saved_stdout), -1);
			if (prev)
				current = prev->next;
			else
				current = *tokens;
			continue ;
		}
		prev = current;
		current = current->next;
	}
	return (0);
}
