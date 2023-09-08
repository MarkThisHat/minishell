/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_fd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maalexan <maalexan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 11:33:47 by inwagner          #+#    #+#             */
/*   Updated: 2023/09/08 19:42:57 by maalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cli	*pipe_fd(t_cli *cli)
{
	if (!cli)
		return (NULL);
	cli->type = PIPE;
	if (pipe(cli->fd) < 0)
	{
		cli->fd[0] = -1;
		cli->fd[1] = -1;
	}
	return (cli->next);
}

/*
**	Makes sure the t_cli node has it's input and
**	output file descriptor properly set
*/
static int	prepare_fd(t_token *tok, int *fd, t_here *heredocs)
{
	t_token	*next;
	char	*file;

	if (!tok || !tok->next || !tok->next->str)
		return (-1);
	next = tok->next;
	file = next->str;
	if (tok->type == HEREDOC)
		fd[0] = heredocs->fd;
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

static void	get_fd(t_token *tok, int *fd, t_here *heredocs)
{
	int		redirector;

	redirector = tok->type;
	if (fd[0] > 0 && (redirector == INPUT))
	{
		close(fd[0]);
		fd[0] = 0;
	}
	if (fd[1] > 0 && (redirector == APPEND || redirector == OVERWRITE))
	{
		close(fd[1]);
		fd[1] = 0;
	}
	if (prepare_fd(tok, fd, heredocs) < 0)
	{
		if ((redirector == INPUT || redirector == HEREDOC))
			fd[0] = -1;
		if ((redirector == APPEND || redirector == OVERWRITE))
			fd[1] = -1;
	}
}
/*
int	assign_each_fd(t_cli *cli, t_token *tok, t_here *heredocs)
{
	print_cli();
	print_token(tok);
	while (tok)
	{
		if (tok->type <= PIPE)
		{
			tok = tok->prev;
			if (tok->next->type == PIPE)
			{
				cli = pipe_fd(cli->next);
				tok = tok->next->next;
				if (heredocs && heredocs->next)
					heredocs = heredocs->next;
			}
			else
				get_fd(tok->next, cli->fd, heredocs);
			if (cli && (cli->fd[0] < 0 || cli->fd[1] < 0))
			{
				if (cli->next)
					remove_cli(cli->next);
				cli = remove_cli(cli);
				tok = discard_tokens(tok);
				get_control()->status = 1;
				if (!tok)
					break ;
			}
			if (get_control()->status == 130)
				return (0);
		}
		tok = tok->next;
	}
	return (1);
}
*/

static t_token	*make_generic(t_token *tok, t_cli *cli, t_here *heredoc)
{
	t_token	*temp;

	if (!tok)
		return (NULL);
	if (!tok->prev && tok->next)
		temp = tok->next->next;
	else
		temp = tok->prev;
	if (cli->fd[0] > -1 && cli->fd[1] > -1)
		prepare_fd(tok, cli->fd, heredoc);
	remove_token(tok->next);
	remove_token(tok);
	return (temp);
}

static t_token	*get_last_redirect(t_type delim, t_token *tok)
{
	t_type	complement;

	if (!delim || !tok)
		return (NULL);
	if (delim == APPEND)
		complement = OVERWRITE;
	if (delim == OVERWRITE)
		complement = APPEND;
	if (delim == HEREDOC)
		complement = INPUT;
	if (delim == INPUT)
		complement = HEREDOC;
	while (tok && tok->type != PIPE)
		tok = tok->next;
	while (tok && tok->type != PIPE)
	{
		if (tok->type == delim || tok->type == complement)
			return (tok);
		tok = tok->prev;
	}
	return (NULL);
}

static	void	make_heredoc(t_token *tok, t_cli *cli, t_here **hdoc)
{
	heredoc = heredoc->next;
}

static int	fill_fd(t_cli *cli, t_token *tok, t_here *heredocs)
{
	if (!cli || !tok)
		return (0);
	while (tok && cli)
	{
		while (tok && tok->type > PIPE)
			tok = tok->next;
		if (tok)
		{
			if (tok->type == HEREDOC)
				make_heredoc();
			else if (tok->type == PIPE)
				cli = cli->next->next;
			else
				make_generic();
		}
	}
}

int	assign_each_fd(t_cli *cli, t_token *tok, t_here *heredocs)
{
	fill_fd(cli, tok, heredocs);
}