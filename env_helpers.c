/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 17:20:00 by jbdmc             #+#    #+#             */
/*   Updated: 2026/06/01 16:12:46 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**gather_envp_array(t_shell *shell, char **envp)
{
	char	**tmp;

	tmp = ft_lsttochpp_no_quotes(envp, shell->env);
	if (!tmp)
	{
		free_envp_array(envp);
		return (NULL);
	}
	return (tmp);
}

char	**build_envp(t_shell *shell)
{
	char	**envp;
	int		size;

	size = env_lstsize(shell->env);
	envp = malloc(sizeof(char *) * (size + 1));
	if (!envp)
		return (NULL);
	return (gather_envp_array(shell, envp));
}

void	free_envp_array(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
		free(envp[i++]);
	free(envp);
}
