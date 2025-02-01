/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:40:08 by hbousset          #+#    #+#             */
/*   Updated: 2025/02/01 11:37:14 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"pipex_bonus.h"

static int	check_syntax(int ac, char **av)
{
	if (!ft_strcmp("here_doc", av[1]))
	{
		if (ac != 6)
			(ft_putstr_fd("invalid structure\n", 2), exit(127));
		return (1);
	}
	return (0);
}

int	main(int ac, char **av, char **env)
{
	if (ac < 5)
	{
		ft_putstr_fd("Error: Invalid number of arguments\n", 2);
		exit(127);
	}
	if (check_syntax(ac, av) == 0)
	{
		handle_pipes(ac, av, env);
	}
}
