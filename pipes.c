/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaulo-b <jpaulo-b@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 12:00:00 by copilot           #+#    #+#             */
/*   Updated: 2026/05/26 11:31:16 by jpaulo-b         ###   ########.fr       */
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
** Extracts a single command from the token list starting at *tokens.
** Returns a pointer to the first token of the next command (after PIPE),
** or NULL if this is the last command. Updates *tokens to point to the
** first token of the extracted command.
*/
t_token	*extract_command(t_token **tokens, t_token **next_cmd)
{
	t_token	*cmd_start;

	cmd_start = *tokens;
	*next_cmd = NULL;
	while (*tokens && (*tokens)->type != PIPE)
		*tokens = (*tokens)->next;
	if (*tokens && (*tokens)->type == PIPE)
	{
		(*tokens)->type = WORD;
		*tokens = (*tokens)->next;
		*next_cmd = *tokens;
	}
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

/*
** Executes a single command in a child process.
** Handles stdin/stdout redirection via prev_read_fd and next_write_fd.
** prev_read_fd: input from the previous command (or -1 if first command)
** next_write_fd: output to the next command (or -1 if last command)
*/
static void	exec_cmd_child(t_token *cmd, int prev_read_fd,
	int next_write_fd, t_shell *shell)
{
	if (prev_read_fd >= 0)
	{
		dup2(prev_read_fd, STDIN_FILENO);
		close_fd(&prev_read_fd);
	}
	if (next_write_fd >= 0)
	{
		dup2(next_write_fd, STDOUT_FILENO);
		close_fd(&next_write_fd);
	}
	get_commands(cmd, shell);
	exit(shell->exit_status);
}

int	execute_pipe_chain(t_token *tokens, t_shell *shell)
{
	int		num_cmds;
	int		i;
	int		pipe_fd[2];
	int		prev_read_fd;
	t_token	*cmd;
	t_token	*next_cmd;
	pid_t	pid;
	int		status;

	num_cmds = count_commands(tokens);
	prev_read_fd = -1;
	i = 0;
	while (i < num_cmds)
	{
		cmd = extract_command(&tokens, &next_cmd);
		if (i < num_cmds - 1)
		{
			if (create_pipe(pipe_fd) == -1)
			{
				close_fd(&prev_read_fd);
				return (1);
			}
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			close_fd(&prev_read_fd);
			if (i < num_cmds - 1)
			{
				close_fd(&pipe_fd[0]);
				close_fd(&pipe_fd[1]);
			}
			return (1);
		}
		if (pid == 0)
		{
			close_fd(&prev_read_fd);
			if (i < num_cmds - 1)
				close_fd(&pipe_fd[0]);
			exec_cmd_child(cmd, prev_read_fd, 
				(i < num_cmds - 1) ? pipe_fd[1] : -1, shell);
		}
		close_fd(&prev_read_fd);
		if (i < num_cmds - 1)
		{
			close_fd(&pipe_fd[1]);
			prev_read_fd = pipe_fd[0];
		}
		tokens = next_cmd;
		i++;
	}
	while (i > 0)
	{
		if (waitpid(-1, &status, 0) > 0)
		{
			if (WIFEXITED(status))
				shell->exit_status = WEXITSTATUS(status);
			else
				shell->exit_status = 1;
		}
		i--;
	}
	return (shell->exit_status);
}
