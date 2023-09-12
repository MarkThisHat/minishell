/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_constructor.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maalexan <maalexan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 11:33:40 by inwagner          #+#    #+#             */
/*   Updated: 2023/09/11 22:32:18 by maalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	treat_fd(t_token *tok, int *fd, int heredoc)
{
	t_token	*next;
	char	*file;

	if (!tok || !tok->next || !tok->next->str)
		return (-1);
	next = tok->next;
	file = next->str;
	if (tok->type == HEREDOC)
		fd[0] = heredoc;
	else if (tok->type == INPUT)
		fd[0] = open(file, O_RDONLY);
	else if (tok->type == APPEND)
		fd[1] = open(file, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
	else if (tok->type == OVERWRITE)
		fd[1] = open(file, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
	if (fd[0] == -1 || fd[1] == -1)
	{
		perror(file);
		return (-1);
	}
	return (0);
}

static t_cli	*define_final_fd(t_cli *cli)
{
	if (!cli)
		return (NULL);
	if (cli->fd[0] != cli->hdoc)
		close(cli->hdoc)
	cli->hdoc = 0;
	return (cli->next);
}

static void	close_unused_fd(int *fd, int heredoc, t_type type)
{
	if (type == APPEND || type == OVERWRITE)
	{
		if (fd[1] > 0)
		{
			close(fd[1]);
			fd[1] = 0;
		}
	}
	if (type == INPUT || type == HEREDOC)
	{
		if (fd[0] > 0)
		{
			if (fd[0] != heredoc)
				close(fd[0]);
			fd[0] = 0;
		}
	}
}

static t_token	*get_next_tok(tok)
{
	t_token	*temp;

	return (NULL);
}

static int	set_fd(t_token *tok, t_cli *cli)
{
	while (cli)
	{
		while (tok && tok->type > PIPE)
			tok = tok->next;
		if (!tok || tok->type == PIPE)
			cli = define_final_fd(cli);
		else if (tok->type < PIPE)
		{
			close_unused_fd(cli->fd, cli->hdoc, tok->type);
			treat_fd(tok, cli->fd, cli->hdoc);
			tok = get_next_tok(tok);
		}
		if (tok)
			tok = tok->next;
	}
}

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
	set_fd(tok, get_control()->commands);
	print_cli();
	exit_program(0);
	return (1);
}
