/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:42:10 by hbousset          #+#    #+#             */
/*   Updated: 2025/02/08 10:16:00 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

static char	**ft_split_quote(const char *s, char c)
{
	char	**results;
	int		tokens;
	int		i;
	int		start;
	int		len;

	if (!s)
		return (NULL);
	start = 0;
	i = 0;
	tokens = count_tokens(s, c);
	results = malloc(sizeof(char *) * (tokens + 1));
	if (!results)
		return (NULL);
	while (i < tokens)
	{
		len = token_length(s, c, start);
		results[i] = malloc(sizeof(char) * (len + 1));
		if (!results[i])
			return (free_split(results, i));
		copy_token(s, c, &start, results[i]);
		i++;
	}
	results[i] = NULL;
	return (results);
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
				perror(px->cmd_args[0]);
				(ft_free(px->paths), ft_free(px->cmd_args), free(full));
				(cleanup(px), free(px), exit(126));
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
				perror(px->cmd_args[0]);
				(ft_free(px->paths), ft_free(px->cmd_args), cleanup(px));
				(free(px), exit(126));
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
		ft_putstr_fd(px->av[px->curr_cmd], 2);
		ft_putstr_fd(":command not found", 2);
		(cleanup(px), free(px), exit(127));
	}
	px->cmd_args = ft_split_quote(px->av[px->curr_cmd], ' ');
	if (!px->cmd_args || !px->cmd_args[0])
	{
		ft_putstr_fd(px->cmd_args[0], 2);
		ft_putstr_fd(":command not found", 2);
		(cleanup(px), free(px), exit(127));
	}
	px->cmd_path = get_cmd_path(px);
	if (!px->cmd_path)
	{
		ft_putstr_fd(px->cmd_args[0], 2);
		ft_putstr_fd(":command not found", 2);
		(ft_free(px->cmd_args), cleanup(px), free(px), exit(127));
	}
	execve(px->cmd_path, px->cmd_args, px->env);
	perror("pipex execve");
	(free(px->cmd_path), ft_free(px->cmd_args), cleanup(px), free(px));
	exit(1);
}
