/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 18:40:00 by jbdmc             #+#    #+#             */
/*   Updated: 2026/06/01 15:24:28 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>

int	heredoc_loop(int write_fd, char *delimiter);

int	create_heredoc_fd(char *delimiter)
{
	int	fd[2];

	if (pipe(fd) == -1)
		return (-1);
	heredoc_loop(fd[1], delimiter);
	close(fd[1]);
	return (fd[0]);
}

int	perform_dup2_and_close(int fd, int target_fd, t_shell *shell)
{
	if (dup2(fd, target_fd) < 0)
	{
		close(fd);
		perror("dup2");
		shell->exit_status = 1;
		return (-1);
	}
	close(fd);
	return (0);
}

void	unlink_token_node(t_token **tokens, t_token *node, t_token *next)
{
	t_token	*iter;

	if (!tokens || !*tokens)
		return ;
	iter = *tokens;
	if (iter == node)
	{
		*tokens = next;
		return ;
	}
	while (iter && iter->next && iter->next != node)
		iter = iter->next;
	if (iter && iter->next == node)
		iter->next = next;
}
