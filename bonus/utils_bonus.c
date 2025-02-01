/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:40:12 by hbousset          #+#    #+#             */
/*   Updated: 2025/02/01 11:22:49 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"pipex_bonus.h"

void	free_pipes(int cmd_count, int **pipes)
{
	int	i;

	i = 0;
	while (i < cmd_count - 1)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

void	close_pipes(int cmd_count, int **pipes)
{
	int	i;

	i = 0;
	while (i < cmd_count - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

int	**alloc_pipes(int cmd_count)
{
	int	**pipes;
	int	i;

	pipes = malloc(sizeof(int *) * (cmd_count - 1));
	if (!pipes)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < cmd_count - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes)
			exit(EXIT_FAILURE);
		if (pipe(pipes[i]) == -1)
			(perror("pipe"), exit(EXIT_FAILURE));
		i++;
	}
	return (pipes);
}

static void	init_first_child(char **av, int **pipes)
{
	int	in;

	in = open(av[1], O_RDONLY);
	if (in < 0)
		(perror("pipex infile"), exit(EXIT_FAILURE));
	if (dup2(in, STDIN_FILENO) == -1 || dup2(pipes[0][1], STDOUT_FILENO) == -1)
		(perror("pipex dup2"), exit(EXIT_FAILURE));
	close(in);
}

static void	mid_child(int i, int **pipes)
{
	if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
	{
		perror("dup2 pipe read");
		exit(EXIT_FAILURE);
	}
	if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
	{
		perror("dup2 pipe write");
		exit(EXIT_FAILURE);
	}
}

void	last_child(char **av, int ac, int cmd_count, int **pipes)
{
	int	out;

	out = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (out < 0)
		(perror("pipex outfile"), exit(EXIT_FAILURE));
	if (dup2(pipes[cmd_count - 2][0], STDIN_FILENO) == -1)
		(perror("pipex dup2"), exit(EXIT_FAILURE));
	if (dup2(out, STDOUT_FILENO) == -1)
		(perror("pipex dup2"), exit(EXIT_FAILURE));
	close(out);
}
void	handle_pipes(int ac, char **av, char **env)
{
	pid_t	*pids;
	int		**pipes;
	int		cmd_count;
	int		i;

	cmd_count = ac - 3;
	pipes = alloc_pipes(cmd_count);
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < cmd_count)
	{
		pids[i] = fork();
		if (pids[i] < 0)
			(perror("fork"), exit(EXIT_FAILURE));
		if (pids[i] == 0)
		{
			if (i == 0)
				init_first_child(av, pipes);
			else if ( i == cmd_count - 1)
				last_child(av, ac, cmd_count, pipes);
			else
				mid_child(i, pipes);
			close_pipes(cmd_count, pipes);
			execute_command_bonus(av[i + 2], env);
		}
		i++;
	}
	close_pipes(cmd_count, pipes);
	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], NULL, 0);
		i++;
	}
	free_pipes(cmd_count, pipes);
	free(pids);
}
