/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getline.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbdmc <jbdmc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 16:50:00 by jbdmc             #+#    #+#             */
/*   Updated: 2026/06/01 16:11:11 by jbdmc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

ssize_t	ft_getline(char **lineptr, size_t *n, FILE *stream)
{
	(void)stream;
	return (ft_getline_fd(lineptr, n, STDIN_FILENO));
}
