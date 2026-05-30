/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 11:19:24 by jbdmc             #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2026/05/30 16:52:04 by jbdmc            ###   ########.fr       */
=======
/*   Updated: 2026/05/30 16:40:13 by jbdmc            ###   ########.fr       */
>>>>>>> 65d1981 (Tiny changes to main)
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

/*
**  Helper function that checks for an empty input line.
*/
static size_t	treat_empty_input(char *line, size_t i)
{
	while (line[i] && (line[i] == ' ' || line[i] == '\t'))
		i++;
	return (i);
}

/*
**  Handle readline input and check for interrupts or empty lines.
**  Returns 1 if processing should continue, 0 if should skip to next iteration.
*/
static int	process_input_line(char **line, t_shell *shell)
{
	if (!*line)
		clean_exit(shell);
	if (g_signal_received)
	{
		shell->exit_status = 130;
		return (0);
	}
	if ((*line)[treat_empty_input(*line, 0)] == '\0')
	{
		free(*line);
		*line = NULL;  /* Mark as freed */
		return (0);
	}
	return (1);
}

/*
**  Process and execute the validated input line.
*/
static void	execute_line(char *line, t_shell *shell, int interactive)
{
	t_token	*tokens;
	char	*validated_line;

	validated_line = read_input_with_continuation(line, shell);
	if (!validated_line)
	{
		return ;
	}
	if (interactive)
		add_history(line);
	tokens = NULL;
	parse_input(validated_line, 0, &tokens, shell);
	free(validated_line);
	free_tokens(tokens);
}

/*
**  Main loop: reads and processes user input continuously.
*/
static void	main_loop(t_shell *shell, int interactive)
{
	char	*line;
	ssize_t	nread;
	size_t	len;
	char	*buf;

	while (1)
	{
		g_signal_received = 0;
		if (interactive)
		{
			line = readline(PROMPT);
			if (!process_input_line(&line, shell))
				continue ;
			execute_line(line, shell, 1);
		}
		else
		{
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
				continue ;
			execute_line(line, shell, 0);
			buf = NULL;
			len = 0;
		}
	}
}

/*
**  main program function: starts the user input request loop and prepares the
** signal handlers for CTRL-D, CTRL-C and CTRL-\.
*/
int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	int		interactive;

	(void)argc;
	(void)argv;
	shell.exit_status = 0;
	init_env(&shell, envp);
	setup_signal_handlers();
	interactive = isatty(STDIN_FILENO);
	main_loop(&shell, interactive);
	return (shell.exit_status);
}
