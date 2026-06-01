/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities_three.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaulo-b <jpaulo-b@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 18:08:07 by jbdmc             #+#    #+#             */
/*   Updated: 2026/06/01 11:17:40 by jpaulo-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*get_env_node(t_env *env_list, char *key)
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

int	env_lstsize(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}

void	free_shell_env(t_shell *shell)
{
	t_env	*tmp;

	while (shell && shell->env)
	{
		tmp = shell->env->next;
		free(shell->env->name);
		free(shell->env->val);
		free(shell->env);
		shell->env = tmp;
	}
}