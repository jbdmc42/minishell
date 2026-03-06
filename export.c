/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaobarb <joaobarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 16:49:55 by joaobarb          #+#    #+#             */
/*   Updated: 2026/03/06 16:15:40 by joaobarb         ###   ########.fr       */
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

/*
**  sorts all the elements in the env struct for the print
*/
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

/*
**  prints the elements of the env struct after being sorted by alpha_sort
*/
void	print_env(t_shell *shell)
{
	char	**var;
	int		i;

	i = 0;
	var = alpha_sort(shell);
	if (!var)
		return ;
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
**  parses and checks the syntax of each argument:
** if it is correct, defines the variable using a split on the token
*/
void	parse_argument(t_token **tokens, t_shell *shell)
{
	char	**nameval;

	nameval = ft_split((*tokens)->value, '=');
	if (!nameval)
		return;
	if (!nameval[0])
	{
		free(nameval);
		return;
	}
	if (!is_valid_var_name(nameval[0]))
	{
		printf("minishell: export: `%s': not a valid identifier\n", nameval[0]);
		free(nameval[0]);
		if (nameval[1])
			free(nameval[1]);
		free(nameval);
		return;
	}
	if (!nameval[1])
		nameval[1] = ft_strdup("");
	process_nameval_quotes(nameval);
	redefine_value(tokens, shell, nameval);
	free(nameval[0]);
	free(nameval[1]);
	free(nameval);
}

/*
**  executes the 'export' command:
** the export command needs at least one WORD type token after the command in
** order to work. 
*/
void	ft_export(t_token **tokens, t_shell *shell)
{
	if ((*tokens)->next == NULL)
	{
		print_env(shell);
		return ;
	}
	*tokens = (*tokens)->next;
	while ((*tokens) != NULL)
	{
		parse_argument(tokens, shell);
		*tokens = (*tokens)->next;
	}
}
