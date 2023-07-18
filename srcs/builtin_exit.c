/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inwagner <inwagner@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 21:07:00 by inwagner          #+#    #+#             */
/*   Updated: 2023/07/17 21:28:28 by inwagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_str_digit(char *arg)
{
	while (arg++)
		if (!ft_isdigit(*arg))
			return (1);
	return (0);
}

int	b_exit(char **args)
{
	if (is_str_digit(args[0]) && !args[1])
		exit_program(0);
	if (is_str_digit(args[0]) && args[1])
		return (ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO), 1);
	if (!is_str_digit(args[0]) && args[1])
	{
		ft_putstr_fd("exit: a: numeric argument required\n", STDERR_FILENO);
		exit_program(2);
	}
	return (0);
}
