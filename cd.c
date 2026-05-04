/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaulo-b <jpaulo-b@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 16:58:26 by jbdmc             #+#    #+#             */
/*   Updated: 2026/05/04 16:58:15 by jpaulo-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env		*get_env_node(t_env *env_list, char *key)
{
	t_env	*current;

	if (!env_list || !key)
		return (NULL);
	current = env_list;
	while (current != NULL)
	{
		if (ft_strcmp(current->name, key) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

char		*get_env_value(t_env *env_list, char *key)
{
	t_env	*node;

	if (!env_list || !key)
		return (NULL);
	node = get_env_node(env_list, key);
	if (node != NULL)
		return (node->val);
	return (NULL);
}

static void	update_env_var(t_shell *data, char *key, char *value)
{
	t_env	*env_node;

	env_node = get_env_node(data->env, key);
	if (env_node)
	{
		free(env_node->val);
		env_node->val = ft_strdup(value);
	}
}

static void	update_pwd_env(t_shell *data, char *old_path)
{
	char	new_path[PATH_MAX];

	update_env_var(data, "OLDPWD", old_path);
	if (getcwd(new_path, sizeof(new_path)) != NULL)
		update_env_var(data, "PWD", new_path);
}

static int	go_to_special_dir(t_shell *data, char *key)
{
	char	*path;
	char	old_path[PATH_MAX];

	path = get_env_value(data->env, key);
	if (!path)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(key, STDERR_FILENO);
		ft_putendl_fd(" not set", STDERR_FILENO);
		return (1);
	}
	if (getcwd(old_path, sizeof(old_path)) == NULL)
		old_path[0] = '\0';
	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	update_pwd_env(data, old_path);
	return (0);
}

int		ft_cd(t_token **tokens, t_shell *shell)
{
	char	old_path[PATH_MAX];

	if (!tokens[1] || ft_strcmp(tokens[1]->value, "~") == 0)
		return (go_to_special_dir(shell, "HOME"));
	if (ft_strcmp(tokens[1]->value, "-") == 0)
		return (go_to_special_dir(shell, "OLDPWD"));
	if (tokens[1]->value && tokens[2])
	{
		ft_putendl_fd("minishell: cd: too many arguments", STDERR_FILENO);
		return (1);
	}
	if (getcwd(old_path, sizeof(old_path)) == NULL)
		old_path[0] = '\0';
	if (chdir(tokens[1]->value) != 0)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(tokens[1]->value, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		return (1);
	}
	update_pwd_env(shell, old_path);
	return (0);
}
