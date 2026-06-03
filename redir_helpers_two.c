/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_helpers_two.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 19:00:00 by jbdmc             #+#    #+#             */
/*   Updated: 2026/06/01 17:21:50 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <unistd.h>

static char	*read_line_from_fd(void)
{
	char	buf[1];
	char	*line;
	char	*new;
	size_t	len;
	ssize_t	r;

	line = NULL;
	len = 0;
	while (1)
	{
		r = read(STDIN_FILENO, buf, 1);
		if (r <= 0)
			break ;
		new = malloc(len + 2);
		if (!new)
			return (free(line), NULL);
		if (line)
		{
			ft_memcpy(new, line, len);
			free(line);
		}
		new[len] = buf[0];
		len++;
		new[len] = '\0';
		line = new;
		if (buf[0] == '\n')
			break ;
	}
	if (r <= 0 && len == 0)
		return (NULL);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	return (line);
}

static char	*get_heredoc_input(void)
{
	char	*line;

	line = NULL;
	if (isatty(STDIN_FILENO))
		line = readline("> ");
	else
	{
		line = read_line_from_fd();
		if (!line)
			return (NULL);
	}
	return (line);
}

int	heredoc_loop(int write_fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = get_heredoc_input();
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	return (0);
}
