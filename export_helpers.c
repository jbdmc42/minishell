/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 11:58:32 by joaobarb          #+#    #+#             */
/*   Updated: 2026/03/10 15:56:39 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_export_nameval(char **nameval, char *arg, char *equals)
{
	if (!equals)
	{
		nameval[0] = ft_strdup(arg);
		nameval[1] = NULL;
	}
	else
	{
		nameval[0] = ft_substr(arg, 0, equals - arg);
		nameval[1] = ft_strdup(equals + 1);
	}
	if (!nameval[0] || (equals && !nameval[1]))
		return (0);
	nameval[2] = NULL;
	return (1);
}

char	**split_export_arg(char *arg)
{
	char	**nameval;
	char	*equals;

	nameval = malloc(sizeof(char *) * 3);
	if (!nameval)
		return (NULL);
	equals = ft_strchr(arg, '=');
	if (!set_export_nameval(nameval, arg, equals))
	{
		if (nameval[0])
			free(nameval[0]);
		if (nameval[1])
			free(nameval[1]);
		free(nameval);
		return (NULL);
	}
	return (nameval);
}

int	fill_node(t_env *var, char *name, char *val, t_env *cur)
{
	if (!val)
		val = "";
	var->name = ft_strdup(name);
	if (!var->name)
		return (0);
	var->val = ft_strdup(val);
	if (!var->val)
	{
		free(var->name);
		return (0);
	}
	var->next = NULL;
	var->prev = cur;
	return (1);
}

void	redefine_value(t_shell *shell, char **nameval)
{
	t_env	*cur;
	char	*new_val;
	char	*val;

	val = nameval[1];
	if (!val)
		val = "";
	cur = shell->env;
	while (cur)
	{
		if (ft_strcmp(cur->name, nameval[0]) == 0)
		{
			new_val = ft_strdup(val);
			if (!new_val)
				return ;
			free(cur->val);
			cur->val = new_val;
			return ;
		}
		cur = cur->next;
	}
	define_value(shell, nameval[0], val);
}

void	define_value(t_shell *shell, char *name, char *val)
{
	t_env	*cur;
	t_env	*var;

	cur = shell->env;
	while (cur && cur->next)
		cur = cur->next;
	var = malloc(sizeof(t_env));
	if (!var)
		return ;
	if (!fill_node(var, name, val, cur))
	{
		free(var);
		return ;
	}
	if (shell->env == NULL)
	{
		shell->env = var;
		var->prev = NULL;
	}
	else
		cur->next = var;
}
