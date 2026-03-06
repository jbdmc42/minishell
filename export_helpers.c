/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 11:58:32 by joaobarb          #+#    #+#             */
/*   Updated: 2026/03/06 10:51:09 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_for_value(t_token **tokens)
{
	int	flag;
	int	i;

	flag = 0;
	i = 0;
	
	while ((*tokens)->value[i])
	{
		if ((*tokens)->value[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

void	redefine_value(t_token **tokens, t_shell *shell, char **nameval)
{
	t_env	*cur;
	int		i;

	cur = shell->env;
	while (cur)
	{
		i = 0;
		while ((*tokens)->value[i])
			i++;
		if ((*tokens)->value[i - 1] == '=' && ft_strcmp(cur->name, nameval[0]))					// case token terminates in '=' the value turns to empty
			cur->val = "";
		else if (ft_strcmp(cur->name, nameval[0]) && check_for_value(tokens))					// case we find the name in the list and theres a value, we redefine it
			cur->val = nameval[1];
		else if (!((*tokens)->value[i - 1] == '=') && !check_for_value(tokens)
				&& (*tokens)->next->value[0] == '=')
		{
			printf("\nminishell: export: `%s':", (*tokens)->next->value);
			printf(" not a valid identifier");
			return ;
		}
		cur = cur->next;
	}
}

void	next_token_is_value(t_token **tokens, t_shell *shell)
{
	
}

void	define_value(t_token **tokens, t_shell *shell)
{
	t_env	*cur;

	cur = shell->env;
	while (cur)
	{
		cur = cur->next;
	}
}