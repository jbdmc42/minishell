/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaobarb <joaobarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 17:40:26 by jbdmc             #+#    #+#             */
/*   Updated: 2026/02/09 11:33:08 by joaobarb         ###   ########.fr       */
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
	if (line[*i] == '|')
	{
		add_token("|", PIPE, tokens);
		(*i)++;
		if (syntaxe_error(line, *i))
			g_global.exit_status = 2;
		return (1);
	}
	return (0);
}

/*
**  less and double less parser: checks the existence of one or two less symbols.
**  Checks the syntaxe to ensure the token can work properly.
**  Also creates a token with the correct type of signal.
*/
int	parse_less(char *line, size_t *i, t_token **tokens)
{
	if (line[*i] == '<')
    {
        if (line[*i + 1] == '<')
        {
            add_token("<<", DLESS, tokens);
            (*i) += 2;
			if (syntaxe_error(line, *i))
				g_global.exit_status = 2;
            return (1);
        }
        add_token("<", LESS, tokens);
        (*i)++;
		if (syntaxe_error(line, *i))
			g_global.exit_status = 2;
        return (1);
    }
    return (0);
}

/*
**  great and double great parser: checks the existence of one or two great symbols.
**  Checks the syntaxe to ensure the token can work properly.
**  Also creates a token with the correct type of signal.
*/
int	parse_great(char *line, size_t *i, t_token **tokens)
{
	if (line[*i] == '>')
    {
        if (line[*i + 1] == '>')
        {
            add_token(">>", DGREAT, tokens);
            (*i) += 2;
			if (syntaxe_error(line, *i))
				g_global.exit_status = 2;
            return (1);
        }
        add_token(">", GREAT, tokens);
        (*i)++;
		if (syntaxe_error(line, *i))
			g_global.exit_status = 2;
        return (1);
    }
    return (0);
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

	if (line[*i] != '\'')	// checks the existence of single quotes
		return (0);
	(*i)++;					// moving past the opening quote
	start = *i;				// defines the beggining of the token content
	while (line[*i] && line[*i] != '\'')		// skip the content and try to find the end of the line or the closing quote
		(*i)++;
	if (line[*i] == '\0')	// detected EOF without a closing quote
	{
		printf("minishell: unexpected EOF while looking for matching `'\n");
		return (-1);
	}
	token = ft_substr(line, start, *i - start);		// token extraction 
	add_token(token, WORD, tokens);					// defining the token value and type	
	free(token);			// as add_token copies the token from the temporary place in char *token, we free it before ending the function
	if (line[*i] == '\'')	// move past the closing quote
		(*i)++;
	return (1);
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

	if (line[*i] != '\"')	// checks the existence of double quotes
		return (0);
	(*i)++;					// moving past the opening quote
	start = *i;				// defines the beggining of the token content
	while (line[*i] && line[*i] != '"')		// skip the content and try to find the end of the line or the closing quote
		(*i)++;
	if (line[*i] == '\0')	// detected EOF without a closing quote
	{
		printf("minishell: unexpected EOF while looking for matching `\"\n");
		return (-1);
	}
	token = ft_substr(line, start, *i - start);		// token extraction
	add_token(token, WORD, tokens);					// defining the token value and type
	free(token);			// as add_token copies the token from the temporary place in char *token, we free it before ending the function
	if (line[*i] == '\"')	// move past the closing quote
		(*i)++;
	return (1);
}
