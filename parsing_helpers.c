/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 17:40:26 by jbdmc             #+#    #+#             */
/*   Updated: 2026/05/30 17:38:01 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**  pipe parser: checks the existence of a pipe symbol.
**  Checks the syntaxe to ensure the token can work properly.
**  Also creates a token with the correct type of signal.
*/
int	parse_pipe(char *line, size_t *i, t_token **tokens, t_shell *shell)
{
	if (line[*i] == '|')
	{
		add_token("|", PIPE, tokens);
		(*i)++;
		if (!syntaxe_error(line, *i))
			shell->exit_status = 2;
		return (1);
	}
	return (0);
}

/*
**  less and double less parser: checks the existence of one or two less symbols.
**  Checks the syntaxe to ensure the token can work properly.
**  Also creates a token with the correct type of signal.
*/
int	parse_less(char *line, size_t *i, t_token **tokens, t_shell *shell)
{
	if (line[*i] == '<')
	{
		if (line[*i + 1] == '<')
		{
			add_token("<<", DLESS, tokens);
			(*i) += 2;
			if (!syntaxe_error(line, *i))
				shell->exit_status = 2;
			return (1);
		}
		add_token("<", LESS, tokens);
		(*i)++;
		if (!syntaxe_error(line, *i))
			shell->exit_status = 2;
		return (1);
	}
	return (0);
}

/*
**  great and double great parser: checks the existence of one or 
** two great symbols.
**  Checks the syntaxe to ensure the token can work properly.
**  Also creates a token with the correct type of signal.
*/
int	parse_great(char *line, size_t *i, t_token **tokens, t_shell *shell)
{
	if (line[*i] == '>')
	{
		if (line[*i + 1] == '>')
		{
			add_token(">>", DGREAT, tokens);
			(*i) += 2;
			if (!syntaxe_error(line, *i))
				shell->exit_status = 2;
			return (1);
		}
		add_token(">", GREAT, tokens);
		(*i)++;
		if (!syntaxe_error(line, *i))
			shell->exit_status = 2;
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

	if (line[*i] != '\'')
		return (0);
	(*i)++;
	start = *i;
	while (line[*i] && line[*i] != '\'')
		(*i)++;
	if (line[*i] == '\0')
	{
		printf("minishell: unexpected EOF while looking for matching `'\n");
		return (-1);
	}
	token = ft_substr(line, start, *i - start);
	add_token(token, WORD, tokens);
	free(token);
	if (line[*i] == '\'')
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

	if (line[*i] != '"')
		return (0);
	(*i)++;
	start = *i;
	while (line[*i] && line[*i] != '"')
		(*i)++;
	if (line[*i] == '\0')
	{
		printf("minishell: unexpected EOF while looking for matching `\"\n");
		return (-1);
	}
	token = ft_substr(line, start, *i - start);
	add_token(token, WORD, tokens);
	free(token);
	if (line[*i] == '"')
		(*i)++;
	return (1);
}
