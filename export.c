/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaobarb <joaobarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 16:49:55 by joaobarb          #+#    #+#             */
/*   Updated: 2026/02/24 17:15:21 by joaobarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(t_shell *shell)
{
	t_env	*cur;

	cur = shell->env;
	write(1, "\n", 1);
	while (cur)
	{
		printf("declare -x ");
		printf("%s", cur->name);
		if (cur->val)
			printf("=\"%s\"\n", cur->val);
		cur = cur->next;
	}
}

/*
**  executes the 'export' command:
** the export command needs at least one WORD type token after the command in
** order to work. 
*/
void	ft_export(t_token **token, t_shell *shell)
{
	if ((*token)->next == NULL)
		print_env(shell);
}
