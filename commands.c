/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaobarb <joaobarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 14:18:09 by jbdmc             #+#    #+#             */
/*   Updated: 2026/02/09 11:44:53 by joaobarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
