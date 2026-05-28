/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helpers_two.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaulo-b <jpaulo-b@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 15:10:00 by joaobarb          #+#    #+#             */
/*   Updated: 2026/05/28 09:38:24 by jpaulo-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static char	*get_env_value_from_shell(t_shell *shell, char *name)
{
	t_env	*current;

	if (!shell || !name)
		return (NULL);
	current = shell->env;
	while (current)
	{
		if (!ft_strcmp(current->name, name))
			return (current->val);
		current = current->next;
	}
	return (NULL);
}

static char	*append_char(char *str, char c)
{
	char	*tmp;
	char	buf[2];

	buf[0] = c;
	buf[1] = '\0';
	tmp = ft_strjoin(str, buf);
	free(str);
	return (tmp);
}

static char	*append_string(char *str, char *add)
{
	char	*tmp;

	tmp = ft_strjoin(str, add);
	free(str);
	return (tmp);
}

static char	*expand_variables(char *segment, t_shell *shell)
{
	size_t	i;
	char	*result;
	char	*name;
	char	*value;
	size_t	start;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (segment[i])
	{
		if (segment[i] == '$')
		{
			start = i + 1;
			if (segment[start] == '\0')
			{
				result = append_char(result, '$');
				break ;
			}
			if (!is_var_char(segment[start]))
			{
				result = append_char(result, '$');
				i++;
				continue ;
			}
			while (segment[start] && is_var_char(segment[start]))
				start++;
			name = ft_substr(segment, i + 1, start - (i + 1));
			if (!name)
			{
				free(result);
				return (NULL);
			}
			value = get_env_value_from_shell(shell, name);
			free(name);
			result = append_string(result, value ? value : "");
			i = start;
		}
		else
		{
			result = append_char(result, segment[i]);
			i++;
		}
	}
	free(segment);
	return (result);
}

static char	*extract_word_part(char *line, size_t *i, t_shell *shell)
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
		if (quote == '\'')
			return (ft_substr(line, start, *i - start - 1));
		return (expand_variables(ft_substr(line, start, *i - start - 1), shell));
	}
	start = *i;
	while (line[*i] && line[*i] != ' ' && line[*i] != '\t'
		&& !is_operator_char(line[*i])
		&& line[*i] != '\'' && line[*i] != '"')
		(*i)++;
	return (expand_variables(ft_substr(line, start, *i - start), shell));
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

void	parse_word(char *line, size_t *i, t_token **tokens, t_shell *shell)
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
		if (!part || !append_word_part(&token, part))
		{
			free(part);
			free(token);
			return ;
		}
		free(part);
	}
	add_token(token, WORD, tokens);
	free(token);
}
