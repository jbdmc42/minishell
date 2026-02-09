/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaobarb <joaobarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 11:00:00 by jbdmc             #+#    #+#             */
/*   Updated: 2026/02/09 10:40:33 by joaobarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Helper function to update quote states
** Tracks whether we're inside single or double quotes
*/
static void	update_quote_state(char c, int *in_single, int *in_double)
{
	if (c == '\'' && !(*in_double))
		*in_single = !(*in_single);
	else if (c == '"' && !(*in_single))
		*in_double = !(*in_double);
}

/*
** Check if quotes are balanced in a string
** Returns 1 if balanced, 0 if not balanced
*/
int	is_quotes_balanced(char *str)
{
	int	i;
	int	in_single;
	int	in_double;

	i = 0;
	in_single = 0;
	in_double = 0;
	while (str && str[i])
	{
		update_quote_state(str[i], &in_single, &in_double);
		i++;
	}
	return (!in_single && !in_double);
}

/**
 * Read input with continuation prompts for unclosed quotes
 * Concatenates multiple lines until quotes are balanced
 */
char	*read_input_with_continuation(void)
{
	char	*line;
	char	*temp;
	char	*continuation;

	line = readline(PROMPT);
	if (!line)
		return (NULL);
	while (!is_quotes_balanced(line))
	{
		continuation = readline("> ");
		if (!continuation)
		{
			free(line);
			return (NULL);
		}
		temp = ft_strjoin(line, "\n");
		free(line);
		line = ft_strjoin(temp, continuation);
		free(temp);
		free(continuation);
	}
	return (line);
}
