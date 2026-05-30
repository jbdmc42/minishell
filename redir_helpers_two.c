/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_helpers_two.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 19:00:00 by jbdmc             #+#    #+#             */
/*   Updated: 2026/05/30 19:01:33 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>

static char	*get_heredoc_input(void)
{
	char	*line;
	ssize_t	nread;
	size_t	len;

	line = NULL;
	len = 0;
	if (isatty(STDIN_FILENO))
		line = readline("> ");
	else
	{
		nread = getline(&line, &len, stdin);
		if (nread == -1)
			return (NULL);
		if (nread > 0 && line[nread - 1] == '\n')
			line[nread - 1] = '\0';
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
