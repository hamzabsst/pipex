/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 18:47:25 by hbousset          #+#    #+#             */
/*   Updated: 2025/02/01 19:50:09 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"pipex_bonus.h"

void	init_first_child(t_pipex *px)
{
	int	in;

	in = open(px->av[1], O_RDONLY);
	if (in < 0)
		(perror("pipex infile"), cleanup(px), exit(EXIT_FAILURE));
	if (dup2(in, 0) == -1 || dup2(px->pipes[0][1], 1) == -1)
		(perror("pipex dup2"), cleanup(px), exit(EXIT_FAILURE));
	close(in);
}

void	mid_child(int i, t_pipex *px)
{
	if (dup2(px->pipes[i - 1][0], STDIN_FILENO) == -1)
		(perror("dup2 pipe read"), cleanup(px), exit(EXIT_FAILURE));
	if (dup2(px->pipes[i][1], STDOUT_FILENO) == -1)
		(perror("dup2 pipe write"), cleanup(px), exit(EXIT_FAILURE));
}

void	last_child(t_pipex *px)
{
	int	out;

	out = open(px->av[px->ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (out < 0)
		(perror("pipex outfile"), cleanup(px), exit(EXIT_FAILURE));
	if (dup2(px->pipes[px->cmd_count - 2][0], STDIN_FILENO) == -1)
		(perror("pipex dup2"), cleanup(px), exit(EXIT_FAILURE));
	if (dup2(out, STDOUT_FILENO) == -1)
		(perror("pipex dup2"), cleanup(px), exit(EXIT_FAILURE));
	close(out);
}
