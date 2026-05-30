/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_extra_two.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 18:45:00 by jbdmc             #+#    #+#             */
/*   Updated: 2026/05/30 18:44:13 by jbdmc            ###   ########.fr       */
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

int	try_exec_in_path(char *path_copy, char *command, char **argv,
		char **envp)
{
	char	full_path[PATH_MAX];
	int		found;

	if (search_in_path_dirs(path_copy, full_path, command, &found) == -1
		&& !found)
		return (-1);
	return (exec_found_command(full_path, argv, envp, NULL));
}

int	exec_direct_path(char *command, char **argv, char **envp, t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		execve(command, argv, envp);
		fprintf(stderr, "%s: %s\n", command, strerror(errno));
		exit(126);
	}
	if (pid < 0)
		return (-1);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else
		shell->exit_status = 1;
	return (0);
}
