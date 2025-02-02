/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_ultils_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 10:16:28 by hbousset          #+#    #+#             */
/*   Updated: 2025/02/02 14:17:01 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"pipex_bonus.h"

char	*get_next(int fd)
{
	char	line[80000];
	int		readbyte;
	char	buff;
	int		i;

	readbyte = 1;
	i = 0;
	buff = 0;
	line[i] = 0;
	while (readbyte)
	{
		readbyte = read(fd, &buff, 1);
		if (readbyte <= 0)
			break ;
		if (buff == '\n')
			break ;
		line[i++] = buff;
		line[i] = 0;
	}
	if (!*line)
		return (NULL);
	return (ft_strdup(line));
}

void	heredoc_first_child(t_pipex px, char *tmp)
{
	int	in;

	in = open(tmp, O_RDONLY);
	if (in < 0)
		(perror("pipex infile"), exit(EXIT_FAILURE));
	if (dup2(in, 0) == -1 || dup2(*(px.pipes[1]), 1) == -1)
		(perror("pipex dup2"), exit(EXIT_FAILURE));
	(close(in), close(*(px.pipes[0])), close(*(px.pipes[1])));
	execute_command_bonus_here_doc(px.av[3], px.env, &px);
}

void	heredoc_second_child(t_pipex px)
{
	int	out;

	out = open(px.av[5], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (out < 0)
		(perror("pipex outfile"), exit(EXIT_FAILURE));
	if (dup2(*(px.pipes[0]), 0) == -1 || dup2(out, 1) == -1)
		(perror("pipex dup2"), exit(EXIT_FAILURE));
	(close(out), close(*(px.pipes[0])), close(*(px.pipes[1])));
	execute_command_bonus_here_doc(px.av[4], px.env, &px);
}

static void	cleanup_heredoc(t_pipex *px)
{
	if (px->pipes)
	{
		close(*px->pipes[0]);
		close(*px->pipes[1]);
	}
}

void	execute_command_bonus_here_doc(char *cmd, char **env, t_pipex *px)
{
	char	**cmd_args;
	char	*cmd_path;

	cmd_args = ft_split(cmd, ' ');
	if (!cmd_args || !cmd_args[0])
	{
		(perror("pipex command not found"), cleanup_heredoc(px));
		(ft_free(cmd_args), exit(127));
	}
	cmd_path = get_command_path(cmd_args[0], env);
	if (!cmd_path)
	{
		(perror("pipex command not found"), cleanup_heredoc(px));
		(ft_free(cmd_args), exit(127));
	}
	execve(cmd_path, cmd_args, env);
	(perror("pipex execve"), cleanup_heredoc(px));
	(free(cmd_path), ft_free(cmd_args), exit(EXIT_FAILURE));
}
