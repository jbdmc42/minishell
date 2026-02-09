/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaobarb <joaobarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 10:00:45 by joaobarb          #+#    #+#             */
/*   Updated: 2026/02/09 10:28:56 by joaobarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**  echo helper function: takes the token right after the echo command and checks
** if that token is a valid '-n' flag.
**  If it is, defines the flag boolean to 0, so that in the end of the echo function
** it will check the value of the boolean, and 0 means it is not supposed to print
** the newline, leaving without ever printing it.
*/
static int	is_valid_n_flag(char *s)
{
	int	i;

	if (!s || s[0] != '-')					// check if the first character is a '-' and if the string is valid
		return (0);
	i = 1;
	if (s[i] == '\0')						// check if the token contains something more than the '-'
		return (0);
	while (s[i] == 'n')						// while the current character is n, we skip
		i++;
	return (s[i] == '\0');					// we return 1 if after the 'n's we ended the string in a '\0' and not any other character. If we don't, we return 0
}

// echo command function
void	ft_echo(t_token **tokens)
{
	int		flag;
	int		first;
	t_token	*cur;

	flag = 1;
	first = 1;
	cur = (*tokens)->next;
	while (cur && cur->type == WORD && is_valid_n_flag(cur->value))			// process -n flags
	{
		flag = 0;
		cur = cur->next;
	}
	while (cur)																// print all arguments following the flags or command
	{
		if (!first)
			write(1, " ", 1);
		write(1, cur->value, ft_strlen(cur->value));
		first = 0;
		cur = cur->next;
	}
	if (flag)
		write(1, "\n", 1);
	return ;
}
