/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaobarb <joaobarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 11:58:32 by joaobarb          #+#    #+#             */
/*   Updated: 2026/03/06 16:16:29 by joaobarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fill_node(t_env *var, char *name, char *val, t_env *cur)
{
	var->name = ft_strdup(name);
	if (!var->name)
		return ;
	var->val = ft_strdup(val);
	if (!var->val)
	{
		free(var->name);
		return ;
	}
	var->next = NULL;
	var->prev = cur;
}

void	redefine_value(t_token **tokens, t_shell *shell, char **nameval)
{
	t_env	*cur;

	(void)tokens;
	cur = shell->env;
	while (cur)
	{
		if (ft_strcmp(cur->name, nameval[0]) == 0)
		{
			free(cur->val);
			cur->val = ft_strdup(nameval[1]);
			return ;
		}
		cur = cur->next;
	}
	define_value(shell, nameval[0], nameval[1]);
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
	fill_node(var, name, val, cur);
	if (shell->env == NULL)
	{
		shell->env = var;
		var->prev = NULL;
	}
	else
		cur->next = var;
}
