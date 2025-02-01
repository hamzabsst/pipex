/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:42:10 by hbousset          #+#    #+#             */
/*   Updated: 2025/02/01 09:48:56 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../inc/pipex.h"

void	*ft_free(char **str)
{
	int	i;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i])
		free (str[i++]);
	free(str);
	return (NULL);
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

static char	*find_command_in_paths(char *cmd, char **paths)
{
	char	*full;
	char	*temp;
	int		i;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			return (ft_free(paths), NULL);
		full = ft_strjoin(temp, cmd);
		free(temp);
		if (!full)
			return (ft_free(paths), NULL);
		if (access(full, X_OK) == 0)
			return (ft_free(paths), full);
		free(full);
		i++;
	}
	return (ft_free(paths), NULL);
}

char	*get_command_path(char *cmd, char **env)
{
	char	**paths;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	paths = get_path_dir(env);
	if (!paths)
		return (NULL);
	return (find_command_in_paths(cmd, paths));
}
