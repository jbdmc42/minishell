/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaulo-b <jpaulo-b@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 16:40:54 by joaobarb          #+#    #+#             */
/*   Updated: 2026/05/12 16:50:39 by jpaulo-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	has_value_in_env_string(char *var_str)
{
	size_t	i;

	i = 0;
	while (var_str[i] && var_str[i] != '=')
		i++;
	if (!var_str[i])
		return (0);
	return (var_str[i + 1] != '\0');
}

static int	env_has_arguments(t_token *tokens, t_shell *shell)
{
	/* If there is no following token, no arguments were provided. */
	if (tokens->next == NULL)
		return (0);
	/* If next token is not a WORD (e.g. PIPE or redirection), it's not an argument. */
	if (tokens->next->type != WORD)
		return (0);
	printf("minishell: env: syntax error near unexpected token `%s'\n",
		tokens->next->value);
	shell->exit_status = 1;
	return (1);
}

static void	print_env_values(char **var)
{
	int	i;

	i = 0;
	while (var[i])
	{
		if (has_value_in_env_string(var[i]))
			printf("%s\n", var[i]);
		i++;
	}
}

static void	free_env_array(char **var)
{
	int	i;

	i = 0;
	while (var[i])
		free(var[i++]);
	free(var);
}

void	ft_env(t_token *tokens, t_shell *shell)
{
	char	**var;

	shell->exit_status = 0;
	if (env_has_arguments(tokens, shell))
		return ;
	var = malloc(sizeof(char *) * (env_lstsize(shell->env) + 1));
	if (!var)
		return ;
	var = ft_lsttochpp_no_quotes(var, shell->env);
	if (!var)
		return ;
	print_env_values(var);
	free_env_array(var);
}
