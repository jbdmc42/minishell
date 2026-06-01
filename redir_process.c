/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 15:40:00 by jbdmc             #+#    #+#             */
/*   Updated: 2026/06/01 15:11:18 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

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

	target = redir->next;
	if (check_target_valid(target, shell) == -1)
		return (-1);
	if (redir->type == DLESS)
		target->value = (char *)(long)create_heredoc_fd(target->value);
	return (apply_redir_by_type(tokens, redir, target, shell));
}

int	process_token_redirections(t_token **tokens, t_shell *shell,
			int saved_stdin, int saved_stdout)
{
	t_token	*current;
	t_token	*next;

	current = *tokens;
	while (current)
	{
		next = current->next;
		if (current->type == LESS || current->type == DLESS
			|| current->type == GREAT || current->type == DGREAT)
		{
			if (handle_one_redirection(tokens, current, shell) == -1)
			{
				restore_redirections(saved_stdin, saved_stdout);
				return (-1);
			}
			current = next;
			continue ;
		}
		current = next;
	}
	return (0);
}
