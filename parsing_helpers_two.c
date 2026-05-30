/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helpers_two.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 15:10:00 by joaobarb          #+#    #+#             */
/*   Updated: 2026/05/30 16:45:32 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *ft_strjoin_free(char *a, char *b)
{
	char *joined;

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

static int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static char	*expand_variable_in_part(char *part, struct s_shell *shell)
{
	size_t  i;
	char    *res;
	char    *name;
	char    *val;
	char    buf[32];

	if (!part)
		return (NULL);
	if (!*part)
		return (ft_strdup(""));
	res = ft_strdup("");
	if (!res)
		return (NULL);
	i = 0;
	while (part[i])
	{
		if (part[i] == '$')
		{
			i++;
			if (part[i] == '?')
			{
				snprintf(buf, sizeof(buf), "%d", shell->exit_status);
				val = ft_strdup(buf);
				res = ft_strjoin_free(res, val);
				i++;
				continue ;
			}
			if (part[i] == '_' || ft_isalpha(part[i]))
			{
				size_t start = i;
				while (part[i] && (part[i] == '_' || ft_isalnum(part[i])))
					i++;
				name = ft_substr(part, start, i - start);
				val = env_get_value(shell->env, name);
				free(name);
				if (!val)
					val = ft_strdup("");
				res = ft_strjoin_free(res, val);
				continue ;
			}
			/* If not valid var char, treat as literal '$' */
			res = ft_strjoin_free(res, ft_substr(part, i - 1, 1));
		}
		else
		{
			char *tmp = ft_substr(part, i, 1);
			res = ft_strjoin_free(res, tmp);
			i++;
		}
	}
	return (res);
}

static char	*extract_word_part(char *line, size_t *i, struct s_shell *shell)
{
	size_t	start;
	char	quote;

	if (line[*i] == '\'' || line[*i] == '"')
	{
		quote = line[*i];
		(*i)++;
		start = *i;
		while (line[*i] && line[*i] != quote)
			(*i)++;
		if (line[*i] == quote)
			(*i)++;
		{
			char *raw = ft_substr(line, start, *i - start - (line[*i - 1] == quote));
			char *out;
			if (quote == '\'')
				out = raw; /* single quotes: no expansion */
			else
			{
				out = expand_variable_in_part(raw, shell);
				free(raw);
			}
			return (out);
		}
	}
	start = *i;
	while (line[*i] && line[*i] != ' ' && line[*i] != '\t'
		&& !is_operator_char(line[*i])
		&& line[*i] != '\'' && line[*i] != '"')
		(*i)++;
	{
		char *raw = ft_substr(line, start, *i - start);
		char *out = expand_variable_in_part(raw, shell);
		free(raw);
		return (out);
	}
}

static int	append_word_part(char **token, char *part)
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

void	parse_word(char *line, size_t *i, t_token **tokens, struct s_shell *shell)
{
	char	*token;
	char	*part;

	token = ft_strdup("");
	if (!token)
		return ;
	while (line[*i] && line[*i] != ' ' && line[*i] != '\t'
		&& !is_operator_char(line[*i]))
	{
		part = extract_word_part(line, i, shell);
		if (!append_word_part(&token, part))
		{
			free(part);
			return ;
		}
		free(part);
	}
	add_token(token, WORD, tokens);
	free(token);
}
