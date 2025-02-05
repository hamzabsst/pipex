/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 08:40:55 by hbousset          #+#    #+#             */
/*   Updated: 2025/02/05 14:03:53 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	create_processes(t_pipex px, char *tmp)
{
	px.pids[0] = fork();
	if (px.pids[0] < 0)
		(perror("pipex fork read end"), exit(EXIT_FAILURE));
	if (px.pids[0] == 0)
		(close(*(px.pipes[0])), heredoc_first_child(px, tmp));
	px.pids[1] = fork();
	if (px.pids[1] < 0)
		(perror("pipex fork write end"), exit(EXIT_FAILURE));
	if (px.pids[1] == 0)
		(close(*(px.pipes[1])), heredoc_second_child(px));
}

void	handle_pipes_heredoc(char **av, char **env, char *tmp)
{
	int		status[2];
	int		pipefd[2];
	int		*pp[2];
	pid_t	pids[2];
	t_pipex	px;

	if (pipe(pipefd) == -1)
		(perror("pipex: pipe"), exit(EXIT_FAILURE));
	pp[0] = &pipefd[0];
	pp[1] = &pipefd[1];
	px.pipes = pp;
	px.av = av;
	px.env = env;
	px.pids = pids;
	create_processes(px, tmp);
	(close(pipefd[0]), close(pipefd[1]));
	(waitpid(pids[0], &status[0], 0), waitpid(pids[1], &status[1], 0));
	if (WIFEXITED(status[1]))
		(unlink(tmp), exit(WEXITSTATUS(status[1])));
	else if (!WIFEXITED(status[0]))
		(unlink(tmp), exit(EXIT_FAILURE));
	else
		(unlink(tmp), exit(WEXITSTATUS(status[0])));
}

static void	write_to_tmp(int fd, char *limiter)
{
	char	*line;
	char	*joined;

	while (1)
	{
		ft_putstr_fd("heredoc> ", 1);
		line = get_next(STDIN_FILENO);
		if (!line)
			break ;
		joined = ft_strjoin(limiter, "\n");
		if (ft_strcmp(line, limiter) == 0 || ft_strcmp(line, joined) == 0)
		{
			(free(line), free(joined));
			break ;
		}
		ft_putstr_fd(line, fd);
		(free(line), free(joined));
	}
}

void	here_doc(char **av, char **env)
{
	int		tmp_fd;
	char	*tmp_filename;

	tmp_filename = "/tmp/.here_doc_tmp";
	tmp_fd = open(tmp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmp_fd < 0)
		(perror("open temporary file"), exit(EXIT_FAILURE));
	write_to_tmp(tmp_fd, av[2]);
	close(tmp_fd);
	handle_pipes_heredoc(av, env, tmp_filename);
}
