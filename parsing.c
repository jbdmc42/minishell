/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 13:54:22 by jbdmc             #+#    #+#             */
/*   Updated: 2026/05/30 17:38:27 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	syntaxe_error(char *line, size_t i)
{
	if (line[i] == '>' || line[i] == '<' || line[i] == '|')
	{
		printf("minishell: syntax error near");
		printf(" unexpected token `%s'\n", &line[i]);
		return (0);
	}
	return (1);
}

int	skip_spaces(char *line, size_t *i)
{
	int	skipped;

	skipped = 0;
	while (line[*i] == ' ' || line[*i] == '\t')
	{
		(*i)++;
		skipped = 1;
	}
	return (skipped);
}

void	parse_input(char *line, size_t i, t_token **tokens, t_shell *shell)
{
	while (line[i] != '\0')
	{
		if (skip_spaces(line, &i))
			continue ;
		if (parse_pipe(line, &i, tokens, shell))
			continue ;
		if (parse_less(line, &i, tokens, shell))
			continue ;
		if (parse_great(line, &i, tokens, shell))
			continue ;
		parse_word(line, &i, tokens, shell);
	}
	get_commands(*tokens, shell);
}
