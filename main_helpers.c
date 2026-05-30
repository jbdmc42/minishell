/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 18:10:00 by jbdmc             #+#    #+#             */
/*   Updated: 2026/05/30 18:12:45 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_input_line(char **line, t_shell *shell)
{
	size_t	start;

	if (!*line)
		clean_exit(shell);
	if (g_signal_received)
	{
		shell->exit_status = 130;
		free(*line);
		*line = NULL;
		return (0);
	}
	start = skip_blank_prefix(*line);
	if ((*line)[start] == '\0')
	{
		free(*line);
		*line = NULL;
		return (0);
	}
	return (1);
}

void	execute_line(char *line, t_shell *shell, int interactive)
{
	t_token	*tokens;
	char	*validated_line;

	validated_line = read_input_with_continuation(line, shell);
	if (interactive)
		add_history(line);
	free(line);
	if (!validated_line)
		return ;
	tokens = NULL;
	parse_input(validated_line, 0, &tokens, shell);
	free(validated_line);
	free_tokens(tokens);
}

int	run_interactive_cycle(t_shell *shell)
{
	char	*line;

	line = readline(PROMPT);
	if (!process_input_line(&line, shell))
		return (0);
	execute_line(line, shell, 1);
	return (1);
}

int	run_noninteractive_cycle(t_shell *shell)
{
	char		*line;
	char		*buf;
	ssize_t		nread;
	size_t		len;

	len = 0;
	buf = NULL;
	nread = getline(&buf, &len, stdin);
	if (nread == -1)
	{
		free(buf);
		clean_exit(shell);
	}
	if (nread > 0 && buf[nread - 1] == '\n')
		buf[nread - 1] = '\0';
	line = buf;
	if (!process_input_line(&line, shell))
		return (0);
	execute_line(line, shell, 0);
	return (1);
}
