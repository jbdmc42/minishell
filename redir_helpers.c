/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 18:40:00 by jbdmc             #+#    #+#             */
/*   Updated: 2026/06/02 12:52:43 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>

int	heredoc_loop(int write_fd, char *delimiter, int should_expand, t_shell *shell);

static void	set_heredoc_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
}

static char	*get_real_delimiter(const char *delimiter)
{
	if (!delimiter)
		return (NULL);
	if ((unsigned char)*delimiter == 0x01)
		return ((char *)(delimiter + 1));
	return ((char *)delimiter);
}

int	create_heredoc_fd(char *delimiter, int should_expand, t_shell *shell)
{
	pid_t    pid;
	int      status;
	struct sigaction    old_int;
	struct sigaction    old_quit;
	struct sigaction    ignore;
	char     template[] = "/tmp/minishell_heredoc_XXXXXX";
	int      tmp_fd;

	tmp_fd = mkstemp(template);
	if (tmp_fd == -1)
		return (-1);
	/* unlink immediately so file is removed when closed */
	unlink(template);
	sigemptyset(&ignore.sa_mask);
	ignore.sa_flags = 0;
	ignore.sa_handler = SIG_IGN;
	sigaction(SIGINT, &ignore, &old_int);
	sigaction(SIGQUIT, &ignore, &old_quit);
	pid = fork();
	if (pid == -1)
	{
		sigaction(SIGINT, &old_int, NULL);
		sigaction(SIGQUIT, &old_quit, NULL);
		close(tmp_fd);
		return (-1);
	}
	if (pid == 0)
	{
		set_heredoc_child_signals();
		/* child writes heredoc content into tmp_fd */
		heredoc_loop(tmp_fd, get_real_delimiter(delimiter), should_expand, shell);
		close(tmp_fd);
		_exit(0);
	}
	status = 0;
	while (1)
	{
		if (waitpid(pid, &status, 0) != -1)
			break ;
		if (errno != EINTR)
		{
			sigaction(SIGINT, &old_int, NULL);
			sigaction(SIGQUIT, &old_quit, NULL);
			close(tmp_fd);
			return (-1);
		}
	}
	sigaction(SIGINT, &old_int, NULL);
	sigaction(SIGQUIT, &old_quit, NULL);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		if (shell)
			shell->exit_status = 130;
		write(STDOUT_FILENO, "\n", 1);
		close(tmp_fd);
		return (-1);
	}
	/* rewind to beginning for readers */
	if (lseek(tmp_fd, 0, SEEK_SET) == -1)
	{
		close(tmp_fd);
		return (-1);
	}
	return (tmp_fd);
}

int	perform_dup2_and_close(int fd, int target_fd, t_shell *shell)
{
	if (dup2(fd, target_fd) < 0)
	{
		close(fd);
		perror("dup2");
		shell->exit_status = 1;
		return (-1);
	}
	close(fd);
	return (0);
}

void	unlink_token_node(t_token **tokens, t_token *node, t_token *next)
{
	t_token	*iter;

	if (!tokens || !*tokens)
		return ;
	iter = *tokens;
	if (iter == node)
	{
		*tokens = next;
		return ;
	}
	while (iter && iter->next && iter->next != node)
		iter = iter->next;
	if (iter && iter->next == node)
		iter->next = next;
}
