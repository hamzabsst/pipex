/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 10:27:25 by hbousset          #+#    #+#             */
/*   Updated: 2025/01/30 14:41:29 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../inc/pipex.h"

static void	execute_command(char *cmd, char **env)
{
	char	**cmd_args;
	char	*cmd_path;

	cmd_args = ft_split(cmd, ' ');
	if (!cmd_args || !cmd_args[0])
	{
		write(2, "pipex: command not found\n", 25);
		ft_free(cmd_args);
		exit(127);
	}
	cmd_path = get_command_path(cmd_args[0], env);
	if (!cmd_path)
	{
		write(2, "pipex: command not found\n", 25);
		ft_free(cmd_args);
		exit(127);
	}
	execve(cmd_path, cmd_args, env);
	perror("pipex");
	free(cmd_path);
	ft_free(cmd_args);
	exit(errno);  // Exit with execve error code
}

static void	first_child(int *pipefd, char **av, char **env)
{
	int	in;

	in = open(av[1], O_RDONLY);
	if (in < 0)
	{
		perror("pipex: infile");
		exit(1);
	}
	if (dup2(in, STDIN_FILENO) == -1 || dup2(pipefd[1], STDOUT_FILENO) == -1)
	{
		perror("pipex: dup2");
		exit(1);
	}
	close(in);
	close(pipefd[0]);
	close(pipefd[1]);
	execute_command(av[2], env);
}

static void	second_child(int *pipefd, char **av, char **env)
{
	int	out;

	out = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (out < 0)
	{
		perror("pipex: outfile");
		exit(1);
	}
	if (dup2(pipefd[0], STDIN_FILENO) == -1 || dup2(out, STDOUT_FILENO) == -1)
	{
		perror("pipex: dup2");
		exit(1);
	}
	close(out);
	close(pipefd[0]);
	close(pipefd[1]);
	execute_command(av[3], env);
}

void	pipex(char **av, char **env)
{
	int		pipefd[2];
	int		status;
	pid_t	pid1, pid2;

	if (pipe(pipefd) == -1)
	{
		perror("pipex: pipe");
		exit(EXIT_FAILURE);
	}
	pid1 = fork();
	if (pid1 < 0)
	{
		perror("pipex: fork");
		exit(EXIT_FAILURE);
	}
	if (pid1 == 0)
	{
		close(pipefd[0]);
		first_child(pipefd, av, env);
	}
	pid2 = fork();
	if (pid2 < 0)
	{
		perror("pipex: fork");
		exit(EXIT_FAILURE);
	}
	if (pid2 == 0)
	{
		close(pipefd[1]);
		second_child(pipefd, av, env);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status))
		exit(WEXITSTATUS(status));
	exit(1);
}
