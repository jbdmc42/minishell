/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaobarb <joaobarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 11:58:32 by joaobarb          #+#    #+#             */
/*   Updated: 2026/03/04 12:39:15 by joaobarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redefine_value(t_token **tokens, t_shell *shell)
{

}

void	next_token_is_value(t_token **tokens, t_shell *shell)
{
	
}

void	define_value(t_token **tokens, t_shell *shell)
{
	t_env	*cur;

	cur = shell->env;
	while (cur)
	{
		cur = cur->next;
	}
}