/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 12:00:00 by copilot           #+#    #+#             */
/*   Updated: 2026/05/30 19:02:01 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Counts the number of commands separated by pipes in the token list.
** Each command is a sequence of tokens ending at a PIPE token or end of list.
*/
int	count_commands(t_token *tokens)
{
	int	count;

	count = 1;
	while (tokens)
	{
		if (tokens->type == PIPE)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

/*
** Extracts a single command from the token list.
** Returns a command token list ending before the next PIPE.
** Does NOT modify the original token list.
** Sets *next_token to point to the next command after the PIPE.
*/
t_token	*extract_command(t_token *tokens, t_token **next_token)
{
	t_token	*cmd_start;
	t_token	*current;
	t_token	*prev;

	cmd_start = tokens;
	*next_token = NULL;
	prev = NULL;
	current = tokens;
	while (current && current->type != PIPE)
	{
		prev = current;
		current = current->next;
	}
	if (current && current->type == PIPE)
		*next_token = current->next;
	if (prev)
		prev->next = NULL;
	return (cmd_start);
}

/*
** Creates a pipe pair (write_fd, read_fd).
** Returns 0 on success, -1 on failure.
*/
int	create_pipe(int *pipe_fd)
{
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	return (0);
}

/*
** Closes a file descriptor if it is >= 0.
*/
void	close_fd(int *fd)
{
	if (*fd >= 0)
	{
		close(*fd);
		*fd = -1;
	}
}
