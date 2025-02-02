/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_ultils_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 10:16:28 by hbousset          #+#    #+#             */
/*   Updated: 2025/02/02 10:18:00 by hbousset         ###   ########.fr       */
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
	execute_command_bonus(px.av[3], px.env, &px);
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
	execute_command_bonus(px.av[4], px.env, &px);
}
