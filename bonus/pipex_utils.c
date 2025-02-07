/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:40:12 by hbousset          #+#    #+#             */
/*   Updated: 2025/02/07 11:39:52 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	init_first_child(t_pipex *px)
{
	int	in;

	px->mode = FULL_CLEANUP;
	in = open(px->av[1], O_RDONLY);
	if (in < 0)
	{
		perror(px->av[1]);
		(cleanup(px), free(px), exit(1));
	}
	if (dup2(in, 0) == -1 || dup2(px->pipes[0][1], 1) == -1)
	{
		(close(in), perror("pipex dup2"));
		(cleanup(px), free(px), exit(1));
	}
	close(in);
}

static void	mid_child(t_pipex *px)
{
	px->mode = FULL_CLEANUP;
	if (dup2(px->pipes[px->curr_cmd - 1][0], STDIN_FILENO) == -1)
		(perror("dup2 pipe read"), cleanup(px), exit(1));
	if (dup2(px->pipes[px->curr_cmd][1], STDOUT_FILENO) == -1)
		(perror("dup2 pipe write"), cleanup(px), exit(1));
}

static void	last_child(t_pipex *px)
{
	int	out;

	out = open(px->av[px->ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	px->mode = FULL_CLEANUP;
	if (out < 0)
	{
		perror(px->av[px->ac - 1]);
		(cleanup(px), free(px), exit(1));
	}
	if (dup2(px->pipes[px->cmd_count - 2][0], STDIN_FILENO) == -1)
	{
		(close(out), perror("pipex dup2"));
		(cleanup(px), free(px), exit(1));
	}
	if (dup2(out, STDOUT_FILENO) == -1)
	{
		(close(out), perror("pipex dup2"));
		(cleanup(px), free(px), exit(1));
	}
	close(out);
}

void	create_child(t_pipex *px)
{
	if (px->curr_cmd == 0)
		init_first_child(px);
	else if (px->curr_cmd == px->cmd_count - 1)
		last_child(px);
	else
		mid_child(px);
	px->mode = CLOSE_PIPES;
	cleanup(px);
	px->mode = FULL_CLEANUP;
	px->curr_cmd = px->curr_cmd + 2;
	execute_command(px);
}

int	wait_for_children(t_pipex *px)
{
	int	i;
	int	status;
	int	last_status;

	last_status = 0;
	i = 0;
	while (i < px->cmd_count)
	{
		if (waitpid(px->pids[i], &status, 0) > 0)
		{
			if (i == px->cmd_count - 1)
			{
				if (WIFEXITED(status))
					last_status = WEXITSTATUS(status);
				else if (WIFSIGNALED(status))
					last_status = 128 + WTERMSIG(status);
			}
		}
		i++;
	}
	return (last_status);
}
