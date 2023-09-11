/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_constructor.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maalexan <maalexan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 11:33:40 by inwagner          #+#    #+#             */
/*   Updated: 2023/09/10 22:40:09 by maalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	build_cli(int count)
{
	t_cli	*cli;

	if (!count)
		return (0);
	cli = malloc(sizeof(t_cli));
	if (!cli)
		return (-1);
	*cli = (t_cli){0};
	get_control()->commands = cli;
	while (--count)
	{
		cli->next = malloc(sizeof(t_cli));
		cli = cli->next;
		if (!cli)
			return (-1);
		*cli = (t_cli){0};
	}
	return (1);
}

static int	count_cli(t_token *tok)
{
	int	count;

	if (!tok)
		return (0);
	count = 1;
	while (tok)
	{
		if (tok->type == PIPE)
			count++;
		tok = tok->next;
	}
	return (count);
}

int	executor_constructor(t_token *tok)
{
	int		count;

	if (!tok)
		return (0);
	count = count_cli(tok);
	if (build_cli(count) < 0)
		exit_program(OUT_OF_MEMORY);
	if (!get_heredoc(tok, get_control()->commands))
		return (0);
	print_cli();
	exit_program(0);
	return (1);
}
