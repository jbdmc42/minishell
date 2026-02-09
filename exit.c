/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaobarb <joaobarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 09:15:11 by joaobarb          #+#    #+#             */
/*   Updated: 2026/02/09 09:28:59 by joaobarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	ft_exit(t_token **tokens)
{
	int		token_count;

	printf("exit\n");
	token_count = count_tokens(*tokens);
	if (token_count > 2)
	{
		printf("minishell: exit: too many arguments\n");
		g_global.exit_status = 1;
		return ;
	}
	if (token_count == 2)
	{
		if (!is_numeric((*tokens)->next->value))
		{
			printf("minishell: exit: %s: numeric argument required\n", 
				(*tokens)->next->value);
			g_global.exit_status = 2;
			free_all();
			exit(g_global.exit_status);
		}
		g_global.exit_status = ft_atoi((*tokens)->next->value);
	}
	free_all();
	exit(g_global.exit_status);
}
