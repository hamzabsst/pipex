/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:42:10 by hbousset          #+#    #+#             */
/*   Updated: 2025/02/07 13:44:32 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	cleanup(t_pipex *px)
{
	int	i;

	if (px->mode == CLOSE_PIPES || px->mode == FULL_CLEANUP)
	{
		i = -1;
		while (++i < px->cmd_count - 1)
			(close(px->pipes[i][0]), close(px->pipes[i][1]));
	}
	if (px->mode == FREE_PIPES || px->mode == FULL_CLEANUP)
	{
		if (px->pipes)
		{
			i = 0;
			while (i < px->cmd_count - 1)
				free(px->pipes[i++]);
			(free(px->pipes), px->pipes = NULL);
		}
	}
	if (px->mode == FULL_CLEANUP)
		if (px->pids)
			(free(px->pids), px->pids = NULL);
	if (px->mode == HEREDOC_CLEANUP)
		if (px->pipes)
			(close(px->pipes[0][0]), close(px->pipes[0][1]));
}

static char	**get_path_dir(char **env)
{
	int	i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (!env[i])
		return (NULL);
	return (ft_split(env[i] + 5, ':'));
}

static char	*find_in_paths(t_pipex *px)
{
	char	*full;
	char	*temp;
	int		i;

	i = -1;
	while (px->paths[++i])
	{
		temp = ft_strjoin(px->paths[i], "/");
		full = ft_strjoin(temp, px->cmd_args[0]);
		free(temp);
		if (!full)
			return (ft_free(px->paths), NULL);
		if (access(full, F_OK) == 0)
		{
			if (access(full, X_OK) != 0)
			{
				(ft_free(px->paths), ft_free(px->cmd_args), cleanup(px));
				(free(full), perror("pipex"), exit(126));
			}
			return (ft_free(px->paths), full);
		}
		free(full);
	}
	return (ft_free(px->paths), NULL);
}

static char	*get_cmd_path(t_pipex *px)
{
	if (!px->cmd_args[0] || !*px->cmd_args[0])
		return (NULL);
	if (ft_strchr(px->cmd_args[0], '/'))
	{
		if (access(px->cmd_args[0], F_OK) == 0)
		{
			if (access(px->cmd_args[0], X_OK) != 0)
			{
				perror("pipex");
				exit(126);
			}
			return (ft_strdup(px->cmd_args[0]));
		}
		return (NULL);
	}
	if (!px->paths)
		px->paths = get_path_dir(px->env);
	if (!px->paths)
		return (NULL);
	return (find_in_paths(px));
}

void	execute_command(t_pipex *px)
{
	if (!px->av[px->curr_cmd] || !*px->av[px->curr_cmd])
	{
		ft_putstr_fd("command not found\n", 2);
		(cleanup(px), free(px), exit(127));
	}
	px->cmd_args = ft_split(px->av[px->curr_cmd], ' ');
	if (!px->cmd_args || !px->cmd_args[0])
	{
		ft_putstr_fd("command not found", 2);
		(cleanup(px), free(px), exit(127));
	}
	px->cmd_path = get_cmd_path(px);
	if (!px->cmd_path)
	{
		ft_putstr_fd("command not found", 2);
		(ft_free(px->cmd_args), cleanup(px), free(px), exit(127));
	}
	execve(px->cmd_path, px->cmd_args, px->env);
	perror("pipex execve");
	(free(px->cmd_path), ft_free(px->cmd_args), cleanup(px), free(px));
	exit(1);
}
