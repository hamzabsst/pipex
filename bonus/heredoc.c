/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 08:40:55 by hbousset          #+#    #+#             */
/*   Updated: 2025/02/07 14:32:55 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	heredoc_first_child(t_pipex *px, char *tmp)
{
	int	in;

	in = open(tmp, O_RDONLY);
	if (in < 0)
		(perror("infile"), exit(EXIT_FAILURE));
	if (dup2(in, 0) == -1 || dup2(*(px->pipes[1]), 1) == -1)
		(perror("dup2"), exit(EXIT_FAILURE));
	(close(in), close(*(px->pipes[0])), close(*(px->pipes[1])));
	px->mode = FULL_CLEANUP;
	px->curr_cmd = px->curr_cmd + 3;
	execute_command(px);
}

static void	heredoc_second_child(t_pipex *px)
{
	int	out;

	out = open(px->av[5], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (out < 0)
		(perror("outfile"), exit(EXIT_FAILURE));
	if (dup2(*(px->pipes[0]), 0) == -1 || dup2(out, 1) == -1)
		(perror("dup2"), exit(EXIT_FAILURE));
	(close(out), close(*(px->pipes[0])), close(*(px->pipes[1])));
	px->mode = FULL_CLEANUP;
	px->curr_cmd = px->curr_cmd + 4;
	execute_command(px);
}

void	create_processes(t_pipex *px, char *tmp)
{
	px->pids[0] = fork();
	if (px->pids[0] < 0)
		(ft_putstr_fd("cannot fork read end", 2), exit(EXIT_FAILURE));
	if (px->pids[0] == 0)
		(close(*(px->pipes[0])), heredoc_first_child(px, tmp));
	px->pids[1] = fork();
	if (px->pids[1] < 0)
		(ft_putstr_fd("cannot fork write end", 2), exit(EXIT_FAILURE));
	if (px->pids[1] == 0)
		(close(*(px->pipes[1])), heredoc_second_child(px));
}

static void	init_heredoc(char **av, char **env, char *tmp)
{
	int		status[2];
	int		pipefd[2];
	int		*pp[2];
	pid_t	pids[2];
	t_pipex	px;

	if (pipe(pipefd) == -1)
		(ft_putstr_fd("cannot create a pipe", 2), exit(EXIT_FAILURE));
	pp[0] = &pipefd[0];
	pp[1] = &pipefd[1];
	px.pipes = pp;
	px.av = av;
	px.env = env;
	px.pids = pids;
	px.cmd_args = NULL;
	px.cmd_path = NULL;
	px.paths = NULL;
	px.curr_cmd = 0;
	create_processes(&px, tmp);
	(close(pipefd[0]), close(pipefd[1]));
	waitpid(pids[0], &status[0], 0);
	waitpid(pids[1], &status[1], 0);
	if (WIFEXITED(status[1]))
		(unlink(tmp), exit(WEXITSTATUS(status[1])));
	(unlink(tmp), exit(EXIT_FAILURE));
}

void	here_doc(char **av, char **env)
{
	int		tmp_fd;
	char	*line;
	char	*joined;

	unlink("/tmp/.here_doc_tmp");
	tmp_fd = open("/tmp/.here_doc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmp_fd < 0)
		(ft_putstr_fd("cannot open tmp file", 2), exit(EXIT_FAILURE));
	joined = ft_strjoin(av[2], "\n");
	if (!joined)
		(close(tmp_fd), unlink("/tmp/.here_doc_tmp"), exit(EXIT_FAILURE));
	while (1)
	{
		write(1, "heredoc> ", 9);
		line = get_next_line(STDIN_FILENO);
		if (!line || !ft_strcmp(line, av[2]) || !ft_strcmp(line, joined))
		{
			free(line);
			break ;
		}
		(ft_putstr_fd(line, tmp_fd), free(line));
	}
	get_next_line(-1);
	(free(joined), close(tmp_fd));
	init_heredoc(av, env, "/tmp/.here_doc_tmp");
}
