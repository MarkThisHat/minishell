/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maalexan <maalexan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 20:49:48 by maalexan          #+#    #+#             */
/*   Updated: 2023/09/11 22:18:16 by maalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_heredoc(t_token	*tok)
{
	while (tok)
	{
		if (tok->type == HEREDOC)
			return (1);
		tok = tok->next;
	}
	return (0);
}
/*
int	free_heredocs(t_here *doc, char closing)
{
	if (!doc)
		return (0);
	free_heredocs(doc->next, closing);
	if (closing)
		close(doc->fd);
	free(doc);
	if (closing)
		return (0);
	return (1);
}
*/