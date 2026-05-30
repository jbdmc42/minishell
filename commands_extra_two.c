/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_extra_two.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaulo-b <jpaulo-b@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 18:45:00 by jbdmc             #+#    #+#             */
/*   Updated: 2026/05/30 19:31:26 by jpaulo-b         ###   ########.fr       */
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
		execve(command, ctx->argv, ctx->envp);
		fprintf(stderr, "%s: %s\n", command, strerror(errno));
		exit(126);
	}
	if (pid < 0)
		return (-1);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		ctx->shell->exit_status = WEXITSTATUS(status);
	else
		ctx->shell->exit_status = 1;
	return (0);
}
