/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaulo-b <jpaulo-b@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 09:15:11 by joaobarb          #+#    #+#             */
/*   Updated: 2026/06/01 12:09:42 by jpaulo-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
**  exit helper function: in short, an atoi function that returns a boolean 
** instead of a converted number. 
**  Checks if the token received if numeric or not.
*/
static int	is_numeric(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/*
**  exit helper function: counts the number of tokens to check if there are 
** more than 2.
*/
static int	count_tokens(t_token *tokens)
{
	int		count;
	t_token	*cur;

	count = 0;
	cur = tokens;
	while (cur)
	{
		count++;
		cur = cur->next;
	}
	return (count);
}

void	ft_exit(t_token *tokens, t_shell *shell)
{
	int		token_count;

	token_count = count_tokens(tokens);
	if (token_count > 2)
	{
		printf("bash: line 1: exit: too many arguments\n");
		shell->exit_status = 1;
		return ;
	}
	if (token_count == 2)
	{
		if (!is_numeric(tokens->next->value))
		{
			printf("bash: line 1: exit: %s: numeric argument required\n",
				tokens->next->value);
			shell->exit_status = 2;
			free_tokens(tokens);
			cleanup_redirections(shell);//
			free_shell_env(shell);
			clean_exit(shell);
		}
		shell->exit_status = ft_atoi(tokens->next->value);
	}
	free_tokens(tokens);
	cleanup_redirections(shell);//
	free_shell_env(shell);
	clean_exit(shell);
}
