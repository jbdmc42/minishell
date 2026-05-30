/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 16:50:42 by joaobarb          #+#    #+#             */
/*   Updated: 2026/05/30 17:31:53 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Remove an environment variable from the shell's env list
static void	remove_env_var(t_shell *shell, char *name)
{
	t_env	*current;
	t_env	*next;

	current = shell->env;
	while (current)
	{
		next = current->next;
		if (ft_strcmp(current->name, name) == 0)
		{
			if (current->prev)
				current->prev->next = current->next;
			else
				shell->env = current->next;
			if (current->next)
				current->next->prev = current->prev;
			free(current->name);
			free(current->val);
			free(current);
		}
		current = next;// Move to the next node in the env list
	}
}

int	ft_unset(t_token *tokens, t_shell *shell)
{
	int		exit_status;
	t_token	*cur;// Pointer to traverse the tokens list

	exit_status = 0;		// When exit_status = 1, application is finished
	if (!tokens)
		return (0);			// If there is no tokens, return 0
	cur = tokens->next;		// The token with the variable name (the 2nd node)
	while (cur)
	{
		if (!is_valid_var_name(cur->value))		// If the var name is not valid
		{
			printf("minishell: unset: `%s': not a valid identifier\n",		// Print error and exit
				cur->value);
			exit_status = 1;
		}
		else						// If the var name is valid, remove it from the env list
		{
			remove_env_var(shell, cur->value);
		}
		cur = cur->next;		// Move to the next token (the next var name)
	}
	shell->exit_status = exit_status;	// Update the shell's exit status
	return (exit_status);
}