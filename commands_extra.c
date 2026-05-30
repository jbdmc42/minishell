/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_extra.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 14:34:55 by jbdmc             #+#    #+#             */
/*   Updated: 2026/05/30 18:56:15 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>

void	free_envp_array(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
		free(envp[i++]);
	free(envp);
}

char	**build_argv(t_token *tokens)
{
	char	**argv;
	t_token	*temp;
	int		count;

	temp = tokens;
	count = 0;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	temp = tokens;
	count = 0;
	while (temp)
	{
		argv[count++] = temp->value;
		temp = temp->next;
	}
	argv[count] = NULL;
	return (argv);
}

char	**build_envp(t_shell *shell)
{
	char	**envp;
	char	**tmp;
	int		size;

	size = env_lstsize(shell->env);
	envp = malloc(sizeof(char *) * (size + 1));
	if (!envp)
		return (NULL);
	tmp = ft_lsttochpp_no_quotes(envp, shell->env);
	if (!tmp)
	{
		free_envp_array(envp);
		return (NULL);
	}
	return (tmp);
}

int	exec_found_command(char *full_path, char **argv, char **envp,
				t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		execve(full_path, argv, envp);
		fprintf(stderr, "%s: %s\n", argv[0], strerror(errno));
		exit(126);
	}
	if (pid < 0)
	{
		return (-1);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else
		shell->exit_status = 1;
	return (0);
}

int	search_and_execute(char *command, char **argv, char **envp, t_shell *shell)
{
	char	*path;
	char	*path_copy;

	if (!command || !argv || !envp)
		return (-1);
	if (ft_strchr(command, '/'))
		return (exec_direct_path(command, argv, envp, shell));
	path = getenv("PATH");
	if (!path)
		return (-1);
	path_copy = ft_strdup(path);
	if (!path_copy)
		return (-1);
	if (try_exec_in_path(path_copy, command, argv, envp) == 0)
	{
		free(path_copy);
		return (0);
	}
	free(path_copy);
	return (-1);
}
