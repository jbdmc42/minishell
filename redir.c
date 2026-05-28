/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaulo-b <jpaulo-b@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 12:00:00 by copilot           #+#    #+#             */
/*   Updated: 2026/05/28 09:34:11 by jpaulo-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>

/* Frees a token node and its value */
static void	free_token_node(t_token *token)
{
	if (!token)
		return ;
	free(token->value);
	free(token);
}

/* Creates a pipe for heredoc content and returns the read end. */
static int	create_heredoc_fd(char *delimiter)
{
	int	fd[2];
	char	*line;

	if (pipe(fd) == -1)
		return (-1);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	close(fd[1]);
	return (fd[0]);
}

/* Opens a file for redirection based on its type */
static int	open_redirection_file(t_token *redir, char *target)
{
	if (redir->type == LESS)
		return (open(target, O_RDONLY));
	if (redir->type == DLESS)
		return (create_heredoc_fd(target));
	if (redir->type == GREAT)
		return (open(target, O_CREAT | O_WRONLY | O_TRUNC, 0644));
	if (redir->type == DGREAT)
		return (open(target, O_CREAT | O_WRONLY | O_APPEND, 0644));
	return (-1);
}

/* Applies a redirection to the shell */
static int	apply_redirection(t_token **tokens, t_token *redir,
	t_shell *shell, t_token **next_token, t_token **prev)
{
	t_token	*target;
	int	fd;
	t_token	*next;

	target = redir->next;
	if (!target || target->type != WORD)
	{
		printf("minishell: syntax error near unexpected token `%s'\n",
			target ? target->value : "newline");
		shell->exit_status = 2;
		return (-1);
	}
	fd = open_redirection_file(redir, target->value);
	if (fd < 0)
	{
		printf("minishell: %s: %s\n", target->value, strerror(errno));
		shell->exit_status = 1;
		return (-1);
	}
	if (redir->type == LESS || redir->type == DLESS)
	{
		if (dup2(fd, STDIN_FILENO) < 0)
		{
			close(fd);
			perror("dup2");
			shell->exit_status = 1;
			return (-1);
		}
	}
	else
	{
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			close(fd);
			perror("dup2");
			shell->exit_status = 1;
			return (-1);
		}
	}
	close(fd);
	next = target->next;
	free_token_node(target);
	if (*prev)
		(*prev)->next = next;
	else
		*tokens = next;
	free_token_node(redir);
	*next_token = next;
	return (0);
}

/* Sets up redirections for the shell */
int	setup_redirections(t_token **tokens, int *saved_stdin,
				int *saved_stdout, t_shell *shell)
{
	t_token	*current;
	t_token	*prev;
	t_token	*next;

	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdin < 0 || *saved_stdout < 0)
	{
		perror("dup");
		return (-1);
	}
	prev = NULL;
	current = *tokens;
	while (current)
	{
		next = current->next;
		if (current->type == LESS || current->type == DLESS
			|| current->type == GREAT || current->type == DGREAT)
		{
			if (apply_redirection(tokens, current, shell, &next, &prev) == -1)
			{
				restore_redirections(*saved_stdin, *saved_stdout);
				return (-1);
			}
			current = next;
			continue ;
		}
		prev = current;
		current = next;
	}
	return (0);
}

/* Restores the original stdin and stdout file descriptors */
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
