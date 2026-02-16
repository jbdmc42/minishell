/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaobarb <joaobarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 18:55:49 by jbdmc             #+#    #+#             */
/*   Updated: 2026/02/12 14:37:48 by joaobarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_token(char *value, t_tokentype type, t_token **tokens)
{
	t_token	*new_token; // New token to be added
	t_token	*tmp_token; // Temporary pointer to traverse list

	new_token = malloc(sizeof(t_token)); // Allocate memory for new token
	if (!new_token) // Check if malloc failed
		return ;
	new_token->value = ft_strdup(value); // Duplicate the token value
	new_token->type = type; // Set token type
	new_token->next = NULL; // Initialize next pointer to NULL
	if (*tokens == NULL) // Check if list is empty
	{
		*tokens = new_token; // Make new token the first token
		return ;
	}
	tmp_token = *tokens; // Start at head of list
	while (tmp_token->next) // Traverse to last token
		tmp_token = tmp_token->next;
	tmp_token->next = new_token; // Add new token at end
}

void	print_tokens(t_token *tokens)
{
	while (tokens) // Iterate through all tokens
	{
		printf("TOKEN: %-12s | TYPE: %d\n", tokens->value, tokens->type); // Print token and type
		tokens = tokens->next; // Move to next token
	}
}
