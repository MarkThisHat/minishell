/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_constructor.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inwagner <inwagner@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 10:34:41 by inwagner          #+#    #+#             */
/*   Updated: 2023/09/08 10:45:03 by inwagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_fd(t_cli *cli, t_token *tok, t_here *heredocs)
{
	int		nodes;
	int		assigned;

	nodes = count_cli(tok);
	while (--nodes)
	{
		cli->next = add_cli(heredocs);
		cli = cli->next;
	}
	cli = get_control()->commands;
	assigned = assign_each_fd(cli, tok, heredocs);
	free_heredocs(heredocs, 0);
	return (assigned);
}

static t_cli	*change_fds(t_cli *before, t_cli *piped, t_cli *after)
{
	if (!before || !piped || !after)
		return (NULL);
	if (before->fd[1] > 2 || after->fd[0] > 2)
	{
		close(piped->fd[0]);
		close(piped->fd[1]);
	}
	else
	{
		before->fd[1] = piped->fd[1];
		after->fd[0] = piped->fd[0];
	}
	free(piped);
	before->next = after;
	return (after);
}

int	pipe_chain(t_cli *cli)
{
	t_cli	*piped;
	t_cli	*after;

	piped = NULL;
	after = NULL;
	while (cli)
	{
		if (cli->next && cli->next->type == PIPE)
		{
			piped = cli->next;
			if (piped->next && piped->next->type != PIPE)
				after = piped->next;
		}
		if (!piped && !after)
			return (1);
		cli = change_fds(cli, piped, after);
		piped = NULL;
		after = NULL;
	}
	return (0);
}

int	executor_constructor(t_token *tok)
{
	t_cli	*cli;
	t_ctrl	*control;
	t_here	*heredocs;

	if (!tok)
		return (0);
	heredocs = get_heredocs(tok);
	control = get_control();
	if (control->status == 130)
	{
		free_heredocs(heredocs, 'c');
		return (0);
	}
	cli = add_cli(heredocs);
	control->commands = cli;
	if (!set_fd(cli, tok, heredocs))
		return (0);
	return (set_cli(control->commands, control->tokens));
}
