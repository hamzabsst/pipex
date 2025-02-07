/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:40:08 by hbousset          #+#    #+#             */
/*   Updated: 2025/02/07 15:02:41 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static int	**alloc_pipes(t_pipex *px)
{
	int	i;

	px->pipes = malloc(sizeof(int *) * (px->cmd_count - 1));
	if (!px->pipes)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < px->cmd_count - 1)
	{
		px->pipes[i] = malloc(sizeof(int) * 2);
		if (!px->pipes[i] || pipe(px->pipes[i]) == -1)
		{
			while (i >= 0)
			{
				close(px->pipes[i][0]);
				close(px->pipes[i][1]);
				free(px->pipes[i]);
				i--;
			}
			(free(px->pipes), perror("pipe"), exit(EXIT_FAILURE));
		}
		i++;
	}
	return (px->pipes);
}

static t_pipex	*init_variables(int ac, char **av, char **env)
{
	t_pipex	*px;

	px = malloc(sizeof(t_pipex));
	if (!px)
		exit(EXIT_FAILURE);
	px->ac = ac;
	px->av = av;
	px->env = env;
	px->cmd_count = ac - 3;
	px->cmd_args = NULL;
	px->cmd_path = NULL;
	px->paths = NULL;
	px->curr_cmd = 0;
	px->pipes = alloc_pipes(px);
	px->pids = malloc(sizeof(pid_t) * px->cmd_count);
	if (!px->pids)
	{
		px->mode = FULL_CLEANUP;
		cleanup(px);
		free(px);
		exit(EXIT_FAILURE);
	}
	return (px);
}

static void	init_pipes(int ac, char **av, char **env)
{
	t_pipex	*px;
	int		last_status;

	px = init_variables(ac, av, env);
	while (px->curr_cmd < px->cmd_count)
	{
		px->pids[px->curr_cmd] = fork();
		if (px->pids[px->curr_cmd] < 0)
		{
			px->mode = FULL_CLEANUP;
			(cleanup(px), free(px), exit(EXIT_FAILURE));
		}
		if (px->pids[px->curr_cmd] == 0)
			create_child(px);
		px->curr_cmd++;
	}
	px->mode = CLOSE_PIPES;
	cleanup(px);
	last_status = wait_for_children(px);
	px->mode = FULL_CLEANUP;
	(cleanup(px), free(px), exit(last_status));
}

int	main(int ac, char **av, char **env)
{
	if (ac < 5 || (!ft_strcmp("here_doc", av[1]) && ac != 6))
	{
		ft_putstr_fd("Error: Invalid number of arguments\n", 2);
		exit(127);
	}
	if (!ft_strcmp("here_doc", av[1]))
		here_doc(av, env);
	else
		init_pipes(ac, av, env);
	exit(EXIT_SUCCESS);
}
