/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaulo-b <jpaulo-b@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 14:18:09 by jbdmc             #+#    #+#             */
/*   Updated: 2026/05/26 15:27:31 by jpaulo-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_builtin_command(char *cmd)
{
    if (!cmd)
        return (0);
    return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "exit")
        || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "env")
        || !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "cd")
        || !ft_strcmp(cmd, "unset"));
}

static void	execute_builtin(t_token *tokens, t_shell *shell)
{
    if (!ft_strcmp(tokens->value, "echo"))
        ft_echo(tokens, shell);
    else if (!ft_strcmp(tokens->value, "exit"))
        ft_exit(tokens, shell);
    else if (!ft_strcmp(tokens->value, "export"))
        ft_export(tokens, shell);
    else if (!ft_strcmp(tokens->value, "env"))
        ft_env(tokens, shell);
    else if (!ft_strcmp(tokens->value, "pwd"))
        ft_pwd();
    else if (!ft_strcmp(tokens->value, "cd"))
        ft_cd(tokens, shell);
    else if (!ft_strcmp(tokens->value, "unset"))
        ft_unset(tokens, shell);
}

static void	handle_external_command(t_token *tokens, t_shell *shell)
{
    char	**argv;
    char	**envp;

    argv = build_argv(tokens);
    if (!argv)
        return ;
    envp = build_envp(shell);
    if (!envp)
    {
        free(argv);
        return ;
    }
    if (search_and_execute(tokens->value, argv, envp) == -1)
    {
        printf("%s: command not found\n", tokens->value);
        shell->exit_status = 127;
    }
    free(argv);
    free_envp_array(envp);
}

void	get_commands(t_token *tokens, t_shell *shell)
{
    int saved_stdin;
    int saved_stdout;
    t_token	*current;

    if (setup_redirections(&tokens, &saved_stdin, &saved_stdout, shell) == -1)
        return ;
    if (!tokens || !tokens->value)
    {
        restore_redirections(saved_stdin, saved_stdout);
        return ;
    }
    current = tokens;
    while (current)
    {
        if (current->type == PIPE)
        {
            execute_pipe_chain(tokens, shell);
            restore_redirections(saved_stdin, saved_stdout);
            return ;
        }
        current = current->next;
    }
    if (is_builtin_command(tokens->value))
        execute_builtin(tokens, shell);
    else
        handle_external_command(tokens, shell);
    restore_redirections(saved_stdin, saved_stdout);
}
