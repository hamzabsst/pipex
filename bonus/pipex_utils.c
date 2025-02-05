/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:40:12 by hbousset          #+#    #+#             */
/*   Updated: 2025/02/05 14:01:25 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	free_pipes(t_pipex *px)
{
	int	i;

	if (!px->pipes)
		return ;
	i = 0;
	while (i < px->cmd_count - 1)
		free(px->pipes[i++]);
	free(px->pipes);
	px->pipes = NULL;
}

int	**alloc_pipes(t_pipex *px)
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
			if (px->pipes[i])
				free(px->pipes[i]);
			while (--i >= 0)
				free(px->pipes[i]);
			free(px->pipes);
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	return (px->pipes);
}

void	create_child(int i, char **env, t_pipex *px)
{
	if (i == 0)
		init_first_child(px);
	else if (i == px->cmd_count - 1)
		last_child(px);
	else
		mid_child(i, px);
	close_pipes(px);
	execute_command_bonus(px->av[i + 2], env, px);
}

int	wait_for_children(t_pipex *px)
{
	int	i;
	int	status;
	int	last_status;

	i = 0;
	while (i < px->cmd_count)
	{
		if (waitpid(px->pids[i], &status, 0) > 0 && i == px->cmd_count - 1)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
		i++;
	}
	return (last_status);
}

