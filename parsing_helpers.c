/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaobarb <joaobarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 17:40:26 by jbdmc             #+#    #+#             */
/*   Updated: 2026/02/12 14:37:48 by joaobarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**  pipe parser: checks the existence of a pipe symbol.
**  Checks the syntaxe to ensure the token can work properly.
**  Also creates a token with the correct type of signal.
*/
int	parse_pipe(char *line, size_t *i, t_token **tokens)
{
	if (line[*i] == '|') // Check if current character is pipe
	{
		add_token("|", PIPE, tokens); // Add pipe token
		(*i)++; // Move to next character
		if (syntaxe_error(line, *i)) // Check for syntax errors after pipe
			g_exit_status = 2; // Set exit status to 2 on error
		return (1); // Return 1 to indicate pipe was parsed
	}
	return (0); // Return 0 if not a pipe
}

/*
**  less and double less parser: checks the existence of one or two less symbols.
**  Checks the syntaxe to ensure the token can work properly.
**  Also creates a token with the correct type of signal.
*/
int	parse_less(char *line, size_t *i, t_token **tokens)
{
	if (line[*i] == '<') // Check if current character is '<'
	{
		if (line[*i + 1] == '<') // Check for '<<' (heredoc)
		{
			add_token("<<", DLESS, tokens); // Add double less token
			(*i) += 2; // Move past both characters
			if (syntaxe_error(line, *i)) // Check for syntax errors
				g_exit_status = 2; // Set exit status to 2 on error
			return (1); // Return 1 to indicate double less was parsed
		}
		add_token("<", LESS, tokens); // Add single less token
		(*i)++; // Move to next character
		if (syntaxe_error(line, *i)) // Check for syntax errors
			g_exit_status = 2; // Set exit status to 2 on error
		return (1); // Return 1 to indicate less was parsed
	}
	return (0); // Return 0 if not a '<'
}

/*
**  great and double great parser: checks the existence of one or 
** two great symbols.
**  Checks the syntaxe to ensure the token can work properly.
**  Also creates a token with the correct type of signal.
*/
int	parse_great(char *line, size_t *i, t_token **tokens)
{
	if (line[*i] == '>') // Check if current character is '>'
	{
		if (line[*i + 1] == '>') // Check for '>>' (append)
		{
			add_token(">>", DGREAT, tokens); // Add double great token
			(*i) += 2; // Move past both characters
			if (syntaxe_error(line, *i)) // Check for syntax errors
				g_exit_status = 2; // Set exit status to 2 on error
			return (1); // Return 1 to indicate double great was parsed
		}
		add_token(">", GREAT, tokens); // Add single great token
		(*i)++; // Move to next character
		if (syntaxe_error(line, *i)) // Check for syntax errors
			g_exit_status = 2; // Set exit status to 2 on error
		return (1); // Return 1 to indicate great was parsed
	}
	return (0); // Return 0 if not a '>'
}

/*
**  single quote parser: checks the existence of single quotes and handles their
** correct or incorrect closure.
**  Also creates a token with the value between the single quotes.
*/
int	parse_single_quotes(char *line, size_t *i, t_token **tokens)
{
	size_t	start;
	char	*token;

	if (line[*i] != '\'') // Check if current character is single quote
		return (0); // Return 0 if not a single quote
	(*i)++; // Move past opening quote
	start = *i; // Mark start of quoted content
	while (line[*i] && line[*i] != '\'') // Find closing quote
		(*i)++; // Move through quoted content
	if (line[*i] == '\0') // Check if reached end without closing quote
	{
		printf("minishell: unexpected EOF while looking for matching `'\n");
		return (-1); // Return -1 on unclosed quote
	}
	token = ft_substr(line, start, *i - start); // Extract content between quotes
	add_token(token, WORD, tokens); // Add token to list
	free(token); // Free temporary token string
	if (line[*i] == '\'') // Move past closing quote
		(*i)++;
	return (1); // Return 1 to indicate single quotes were parsed
}

/*
**  double quote parser: checks the existence of double quotes and handles their
** correct or incorrect closure.
**  Also creates a token with the value between the double quotes.
*/
int	parse_double_quotes(char *line, size_t *i, t_token **tokens)
{
	size_t	start;
	char	*token;

	if (line[*i] != '"') // Check if current character is double quote
		return (0); // Return 0 if not a double quote
	(*i)++; // Move past opening quote
	start = *i; // Mark start of quoted content
	while (line[*i] && line[*i] != '"') // Find closing quote
		(*i)++; // Move through quoted content
	if (line[*i] == '\0') // Check if reached end without closing quote
	{
		printf("minishell: unexpected EOF while looking for matching `\"\n");
		return (-1); // Return -1 on unclosed quote
	}
	token = ft_substr(line, start, *i - start); // Extract content between quotes
	add_token(token, WORD, tokens); // Add token to list
	free(token); // Free temporary token string
	if (line[*i] == '"') // Move past closing quote
		(*i)++;
	return (1); // Return 1 to indicate double quotes were parsed
}
