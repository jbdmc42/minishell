/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 18:55:46 by jbdmc             #+#    #+#             */
/*   Updated: 2026/02/28 12:42:26 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static t_env	*create_env_node(char *envp_entry)
{
	t_env	*new_node;
	char	*equals;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	equals = ft_strchr(envp_entry, '=');
	if (equals)
	{
		new_node->name = ft_substr(envp_entry, 0, equals - envp_entry);
		new_node->val = ft_strdup(equals + 1);
	}
	else
	{
		new_node->name = ft_strdup(envp_entry);
		new_node->val = ft_strdup("");
	}
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

static void	add_env_node(t_env **head, t_env *new_node)
{
	t_env	*current;

	if (!*head)
	{
		*head = new_node;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_node;
	new_node->prev = current;
}

void	init_env(t_shell *shell, char **envp)
{
	int		i;

	shell->env = NULL;
	i = 0;
	while (envp[i])
	{
		add_env_node(&shell->env, create_env_node(envp[i]));
		i++;
	}
}
