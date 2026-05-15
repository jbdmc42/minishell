/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaulo-b <jpaulo-b@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 16:50:42 by joaobarb          #+#    #+#             */
/*   Updated: 2026/05/14 17:24:25 by jpaulo-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Remove an environment variable from the shell's env list
static void	remove_env_var(t_shell *shell, char *name)
{
	t_env	*current;	// Pointers to traverse the env list
	t_env	*next;

	current = shell->env;	// Start from the head of the env list
	while (current)
	{
		next = current->next;	// Store next pointer before freeing current node
		if (ft_strcmp(current->name, name) == 0)// If current env var matches name to unset
		{
			if (current->prev)// If current node is not the head, update previous node's next pointer
				current->prev->next = current->next;// Update previous node's next pointer
			else	// If current node is the head, update shell's env pointer to the next node
				shell->env = current->next;
			if (current->next)// If current node is not the tail, update next node's previous pointer
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