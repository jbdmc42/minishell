/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 16:49:55 by joaobarb          #+#    #+#             */
/*   Updated: 2026/02/28 12:44:53 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_swap(char **a, char **b)
{
	char	*tmp;
	
	tmp = *a;
	*a = *b;
	*b = tmp;
}

char	**alpha_sort(t_shell *shell)
{
	t_env	*cur;
	char	**var;
	int		i;
	int		j;
	
	i = 0;
	cur = shell->env;
	var = malloc(sizeof(char *) * ((env_lstsize(shell->env)) + 1));
	if (!var)
		return (NULL);
	var = ft_lsttochpp(var, cur);
	i = 0;
	while (i < (env_lstsize(shell->env)) - 1)
	{
		j = 0;
		while (j < (env_lstsize(shell->env)) - i - 1)
		{
			if (ft_strcmp(var[j], var[j + 1]) > 0)
				ft_swap(&var[j], &var[j + 1]);
			j++;
		}
		i++;
	}
	return (var);
}

void	print_env(t_shell *shell)
{
	char	**var;
	int		i;

	i = 0;
	var = alpha_sort(shell);
	while (var[i])
	{
		printf("declare -x %s\n", var[i]);
		i++;
	}
	i = 0;
	while (var[i])
		free(var[i++]);
	free(var);
}

/*
**  executes the 'export' command:
** the export command needs at least one WORD type token after the command in
** order to work. 
*/
void	ft_export(t_token **token, t_shell *shell)
{
	if ((*token)->next == NULL)
		print_env(shell);
}
