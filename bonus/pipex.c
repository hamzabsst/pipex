/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:40:08 by hbousset          #+#    #+#             */
/*   Updated: 2025/02/05 13:41:43 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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

static void	handle_pipes(int ac, char **av, char **env)
{
	t_pipex	px;
	int		i;
	int		last_status;

	px.ac = ac;
	px.av = av;
	px.env = env;
	px.cmd_count = ac - 3;
	px.pipes = alloc_pipes(&px);
	px.pids = malloc(sizeof(pid_t) * px.cmd_count);
	if (!px.pids)
		(cleanup(&px), exit(EXIT_FAILURE));
	i = 0;
	while (i < px.cmd_count)
	{
		px.pids[i] = fork();
		if (px.pids[i] < 0)
			(perror("fork"), cleanup(&px), exit(EXIT_FAILURE));
		if (px.pids[i] == 0)
			create_child(i, env, &px);
		i++;
	}
	close_pipes(&px);
	last_status = wait_for_children(&px);
	(cleanup(&px), exit(last_status));
}

int	main(int ac, char **av, char **env)
{
	if (ac < 5)
	{
		ft_putstr_fd("Error: Invalid number of arguments\n", 2);
		exit(127);
	}
	if (check_syntax(ac, av) == 0)
		handle_pipes(ac, av, env);
	else
		here_doc(av, env);
	exit(EXIT_SUCCESS);
}
