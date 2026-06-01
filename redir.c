/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 12:00:00 by copilot           #+#    #+#             */
/*   Updated: 2026/05/30 18:50:49 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>

static int	apply_redir_by_type(t_token **tokens, t_token *redir,
		t_token *target, t_shell *shell)
{
	if (redir->type == LESS || redir->type == DLESS)
		return (apply_redirection_stdin(tokens, redir, target, shell));
	else
		return (apply_redirection_stdout(tokens, redir, target, shell));
}

static int	handle_one_redirection(t_token **tokens, t_token *redir,
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

static int	process_token_redirections(t_token **tokens, t_shell *shell,
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

int	setup_redirections(t_token **tokens, int *saved_stdin,
			int *saved_stdout, t_shell *shell)
{
	if (!tokens || !*tokens)
		return (0);
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdin < 0 || *saved_stdout < 0)
	{
		perror("dup");
		return (-1);
	}
	return (process_token_redirections(tokens, shell, *saved_stdin,
			*saved_stdout));
}

void	restore_redirections(int saved_stdin, int saved_stdout)
{
	if (saved_stdin >= 0)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout >= 0)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}

void	store_saved_redirections(t_shell *shell, int saved_stdin, int saved_stdout)
{
	if (!shell)
		return ;
	shell->saved_stdin = saved_stdin;
	shell->saved_stdout = saved_stdout;
	shell->redirs_saved = 1;
}

void	cleanup_redirections(t_shell *shell)
{
	if (!shell || !shell->redirs_saved)
		return ;
	restore_redirections(shell->saved_stdin, shell->saved_stdout);
	shell->saved_stdin = -1;
	shell->saved_stdout = -1;
	shell->redirs_saved = 0;
}
