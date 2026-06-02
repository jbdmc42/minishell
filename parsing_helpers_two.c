/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helpers_two.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 15:10:00 by joaobarb          #+#    #+#             */
/*   Updated: 2026/06/02 10:00:07 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_free(char *a, char *b)
{
	char	*joined;

	if (!a && !b)
		return (NULL);
	if (!a)
	{
		joined = ft_strdup(b);
		free(b);
		return (joined);
	}
	if (!b)
		return (a);
	joined = ft_strjoin(a, b);
	free(a);
	free(b);
	return (joined);
}

int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	append_word_part(char **token, char *part)
{
	char	*joined;

	joined = ft_strjoin(*token, part);
	if (!joined)
	{
		free(*token);
		return (0);
	}
	free(*token);
	*token = joined;
	return (1);
}

void	parse_word(char *line, size_t *i, t_token **tokens,
	t_shell *shell)
{
	char	*token;
	char	*part;
	int		started_with_quote;

	token = ft_strdup("");
	if (!token)
		return ;
	started_with_quote = (line[*i] == '\'' || line[*i] == '"');
	while (line[*i] && line[*i] != ' ' && line[*i] != '\t'
		&& !is_operator_char(line[*i]))
	{
		part = extract_word_part(line, i, shell);
		if (!append_word_part(&token, part))
		{
			free(part);
			free(token);
			return ;
		}
		free(part);
	}
	if (started_with_quote && token)
	{
		char	*marked;
		marked = malloc(ft_strlen(token) + 2);
		if (marked)
		{
			marked[0] = 0x01;
			ft_strlcpy(marked + 1, token, ft_strlen(token) + 1);
			free(token);
			token = marked;
		}
	}
	add_token(token, WORD, tokens);
	free(token);
}
