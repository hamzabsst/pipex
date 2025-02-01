/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 18:45:33 by hbousset          #+#    #+#             */
/*   Updated: 2025/02/01 19:41:32 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"pipex_bonus.h"

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

void	close_pipes(t_pipex *px)
{
	int	i;

	i = 0;
	while (i < px->cmd_count - 1)
	{
		close(px->pipes[i][0]);
		close(px->pipes[i][1]);
		i++;
	}
}

void	cleanup(t_pipex *px)
{
	if (px->pipes)
		free_pipes(px);
	if (px->pids)
	{
		free(px->pids);
		px->pids = NULL;
	}
}
