/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 10:27:25 by hbousset          #+#    #+#             */
/*   Updated: 2025/02/03 09:02:45 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

static void	execute_command(char *cmd, char **env)
{
	char	**cmd_args;
	char	*cmd_path;

	cmd_args = ft_split(cmd, ' ');
	if (!cmd_args || !cmd_args[0])
		(perror("pipex command not found"), ft_free(cmd_args), exit(127));
	cmd_path = get_command_path(cmd_args[0], env);
	if (!cmd_path)
		(perror("pipex command not found"), ft_free(cmd_args), exit(127));
	execve(cmd_path, cmd_args, env);
	perror("pipex execve");
	(free(cmd_path), ft_free(cmd_args), exit(EXIT_FAILURE));
}

static void	first_child(int *pipefd, char **av, char **env)
{
	int	in;

	in = open(av[1], O_RDONLY);
	if (in < 0)
		(perror("pipex infile"), exit(EXIT_FAILURE));
	if (dup2(in, STDIN_FILENO) == -1 || dup2(pipefd[1], STDOUT_FILENO) == -1)
		(perror("pipex dup2"), exit(EXIT_FAILURE));
	(close(in), close(pipefd[0]), close(pipefd[1]));
	execute_command(av[2], env);
}

static void	second_child(int *pipefd, char **av, char **env)
{
	int	out;

	out = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (out < 0)
		(perror("pipex outfile"), exit(EXIT_FAILURE));
	if (dup2(pipefd[0], STDIN_FILENO) == -1 || dup2(out, STDOUT_FILENO) == -1)
		(perror("pipex dup2"), exit(EXIT_FAILURE));
	(close(out), close(pipefd[0]), close(pipefd[1]));
	execute_command(av[3], env);
}

static void	create_processes(int *pipefd, char **av, char **env, pid_t *pids)
{
	pids[0] = fork();
	if (pids[0] < 0)
		(perror("pipex fork read end"), exit(EXIT_FAILURE));
	if (pids[0] == 0)
		(close(pipefd[0]), first_child(pipefd, av, env));
	pids[1] = fork();
	if (pids[1] < 0)
		(perror("pipex fork write end"), exit(EXIT_FAILURE));
	if (pids[1] == 0)
		(close(pipefd[1]), second_child(pipefd, av, env));
}

void	pipex(char **av, char **env)
{
	int		pipefd[2];
	int		status1;
	int		status2;
	pid_t	pids[2];

	if (pipe(pipefd) == -1)
		(perror("pipex: pipe"), exit(EXIT_FAILURE));
	create_processes(pipefd, av, env, pids);
	(close(pipefd[0]), close(pipefd[1]));
	(waitpid(pids[0], &status1, 0), waitpid(pids[1], &status2, 0));
	if (WIFEXITED(status2))
		exit(WEXITSTATUS(status2));
	else if (!WIFEXITED(status1))
		exit(EXIT_FAILURE);
	else
		exit(WEXITSTATUS(status1));
}
