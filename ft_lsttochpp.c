/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lsttochpp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 11:45:58 by jbdmc             #+#    #+#             */
/*   Updated: 2026/02/28 12:20:57 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_lsttochpp(char **chpp, t_env *env)
{
	char	*tmp_st;
	char	*tmp_nd;
	int		i;
	
	i = 0;
	while (env)
	{
		tmp_st = ft_strjoin(env->name, "=\"");
		if (!tmp_st)
			return (NULL);
		tmp_nd = ft_strjoin(tmp_st, env->val);
		free(tmp_st);
		if (!tmp_nd)
			return (NULL);
		chpp[i] = ft_strjoin(tmp_nd, "\"");
		free(tmp_nd);
		if (!chpp[i])
			return (NULL);
		i++;
		env = env->next;
	}
	chpp[i] = NULL;
	return (chpp);
}
