/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_extra_two.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 18:45:00 by jbdmc             #+#    #+#             */
/*   Updated: 2026/06/01 14:42:27 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>
#include <errno.h>

static int	search_in_path_dirs(char *path_copy, char *full_path,
		char *command, int *found)
{
	char	*dir;

	dir = strtok(path_copy, ":");
	while (dir)
	{
		snprintf(full_path, PATH_MAX, "%s/%s", dir, command);
		if (access(full_path, X_OK) == 0)
		{
			*found = 1;
			return (0);
		}
		dir = strtok(NULL, ":");
	}
	*found = 0;
	return (-1);
}

int	try_exec_in_path(char *path_copy, char *command, t_exec_ctx *ctx)
{
	char	full_path[PATH_MAX];
	int		found;

	if (search_in_path_dirs(path_copy, full_path, command, &found) == -1
		&& !found)
		return (-1);
	return (exec_found_command(full_path, command, ctx));
}

int	exec_direct_path(char *command, t_exec_ctx *ctx)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execve(command, ctx->argv, ctx->envp);
		fprintf(stderr, "%s: %s\n", command, strerror(errno));
		exit(126);
	}
	if (pid < 0)
		return (-1);
	status = 0;
	while (waitpid(pid, &status, 0) == -1 && errno == EINTR)
		;
	if (WIFEXITED(status))
		ctx->shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		ctx->shell->exit_status = 128 + WTERMSIG(status);
	else
		ctx->shell->exit_status = 1;
	return (0);
}

void	finalize_processing(t_token *tokens, t_shell *shell,
	int saved_stdin, int saved_stdout)
{
	free_tokens(tokens);
	restore_redirections(saved_stdin, saved_stdout);
	shell->saved_stdin = -1;
	shell->saved_stdout = -1;
	shell->redirs_saved = 0;
}

int	setup_and_store(t_token **tokens, t_shell *shell,
	int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = -1;
	*saved_stdout = -1;
	if (setup_redirections(tokens, saved_stdin, saved_stdout, shell) == -1)
	{
		if (*saved_stdin >= 0)
			close(*saved_stdin);
		if (*saved_stdout >= 0)
			close(*saved_stdout);
		free_tokens(*tokens);
		shell->saved_stdin = -1;
		shell->saved_stdout = -1;
		shell->redirs_saved = 0;
		return (-1);
	}
	store_saved_redirections(shell, *saved_stdin, *saved_stdout);
	return (0);
}
