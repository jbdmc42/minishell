/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaobarb <joaobarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 09:15:11 by joaobarb          #+#    #+#             */
/*   Updated: 2026/02/09 11:50:08 by joaobarb         ###   ########.fr       */
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

// exit command function
void	ft_exit(t_token **tokens)
{
	int		token_count;

	printf("exit\n");
	token_count = count_tokens(*tokens);
	if (token_count > 2)	// check if there are more than 2 arguments because exit only takes 1 argument
	{
		printf("minishell: exit: too many arguments\n");
		g_global.exit_status = 1;
		return ;
	}
	if (token_count == 2)	// check if the argument is numeric
	{
		if (!is_numeric((*tokens)->next->value))	// if is isn't, do the exit and display the following message
		{
			printf("minishell: exit: %s: numeric argument required\n", 
				(*tokens)->next->value);
			g_global.exit_status = 2;
			free_all();
			exit(g_global.exit_status);
		}
		g_global.exit_status = ft_atoi((*tokens)->next->value);		// if it is, define the exit status to the numeric value of the token using atoi
	}
	free_all();
	exit(g_global.exit_status);
}
