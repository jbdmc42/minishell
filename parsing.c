/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 13:54:22 by jbdmc             #+#    #+#             */
/*   Updated: 2026/05/30 17:30:24 by jbdmc            ###   ########.fr       */
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
	int	skipped; // Flag to track if any spaces were skipped

	skipped = 0; // Initialize to 0 (no spaces skipped yet)
	while (line[*i] == ' ' || line[*i] == '\t') // Skip spaces and tabs
	{
		(*i)++; // Move to next character
		skipped = 1; // Mark that we skipped at least one space
	}
	return (skipped); // Return 1 if spaces were skipped, 0 otherwise
}

void	parse_input(char *line, size_t i, t_token **tokens, t_shell *shell)
{
	while (line[i] != '\0') // Loop through entire input line
	{
		if (skip_spaces(line, &i)) // Skip whitespace
			continue ; // Continue to next iteration
		if (parse_pipe(line, &i, tokens, shell)) // Try parsing pipe
			continue ; // Continue if pipe was parsed
		if (parse_less(line, &i, tokens, shell)) // Try parsing less/heredoc
			continue ; // Continue if less was parsed
		if (parse_great(line, &i, tokens, shell)) // Try parsing great/append
			continue ; // Continue if great was parsed
<<<<<<< HEAD
		parse_word(line, &i, &tokens, shell); // Parse regular word
=======
		parse_word(line, &i, tokens, shell); // Parse regular word (with expansion)
>>>>>>> d0c3708 (Fixed double free on prompt line.)
	}
	get_commands(*tokens, shell); // Execute the parsed commands
}
