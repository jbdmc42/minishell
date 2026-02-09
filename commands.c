/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaobarb <joaobarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 14:18:09 by jbdmc             #+#    #+#             */
/*   Updated: 2026/02/09 09:15:45 by joaobarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo(t_token **tokens)
{
	int		flag;
	int		first;
	t_token	*cur;

	flag = 1;
	first = 1;
	cur = (*tokens)->next;
	while (cur && cur->type == WORD && is_valid_n_flag(cur->value))			// process -n flags
	{
		flag = 0;
		cur = cur->next;
	}
	while (cur)																// print all arguments following the flags or command
	{
		if (!first)
			write(1, " ", 1);
		write(1, cur->value, ft_strlen(cur->value));
		first = 0;
		cur = cur->next;
	}
	if (flag)
		write(1, "\n", 1);
	return ;
}

/* void	ft_cd(t_token **tokens)
{
	
}

void	ft_pwd(t_token **tokens)
{
	
}

void	ft_export(t_token **tokens)
{
	
} */

void	get_commands(t_token **tokens)
{
	if (!tokens || !*tokens || !(*tokens)->value)
		return ;	
	if (ft_strcmp((*tokens)->value, "echo") == 0)
        ft_echo(tokens);
   /*  else if (ft_strcmp((*tokens)->value, "cd") == 0)
        ft_cd(tokens);
    else if (ft_strcmp((*tokens)->value, "pwd") == 0)
        ft_pwd(tokens);
    else if (ft_strcmp((*tokens)->value, "export") == 0)
        ft_export(tokens);
    else if (ft_strcmp((*tokens)->value, "unset") == 0)
        ft_unset(tokens);
    else if (ft_strcmp((*tokens)->value, "env") == 0)
        ft_env(tokens); */
    else if (ft_strcmp((*tokens)->value, "exit") == 0)
        ft_exit(tokens);
    else
    {
        printf("%s: command not found\n", (*tokens)->value);
        g_global.exit_status = 127;
    }
}
