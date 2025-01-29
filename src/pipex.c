/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 10:27:25 by hbousset          #+#    #+#             */
/*   Updated: 2025/01/29 08:39:34 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../inc/pipex.h"

static void	execute_command(char *cmd, char **env, char *error_msg)
{
	char	**cmd_args;
	char	*cmd_path;

	if (!cmd || !*cmd)
		exit(write(2, "Error: empty command\n", 20));
	cmd_args = ft_split(cmd, ' ');
	cmd_path = get_command_path(cmd_args[0], env);
	if (!cmd_path)
	{
		free(cmd_path);
		exit(write(2, "Error: command not found\n", 24));
	}
	execve(cmd_path, cmd_args, env);
	free(cmd_path);
	ft_free(cmd_args);
	exit(write(2, error_msg, ft_strlen(error_msg)));
}

static void	first_child(int *pipefd, char **av, char **env)
{
	int	in;

	in = open(av[1], O_RDONLY);
	if (in < 0)
		exit(write(2, "Error: can not open infile\n", 27));
	dup2(in, STDIN_FILENO);
	close(in);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
	execute_command(av[2], env, "Error: failed to execute cmd1\n");
}

static void	second_child(int *pipefd, char **av, char **env)
{
	int	out;

	out = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (out < 0)
		exit(write(2, "Error: can not open outfile\n", 28));
	dup2(pipefd[0], STDIN_FILENO);
	dup2(out, STDOUT_FILENO);
	close(out);
	close(pipefd[0]);
	close(pipefd[1]);
	execute_command(av[3], env, "Error: failed to execute cmd2\n");
}

void	pipex(char **av, char **env)
{
	int		pipefd[2];
	int		status1;
	int		status2;
	pid_t	pid1;
	pid_t	pid2;

	if (pipe(pipefd) == -1)
		exit(write(2, "Error: can not create pipe\n", 27));
	pid1 = fork();
	if (pid1 == 0)
		first_child(pipefd, av, env);
	else if (pid1 < 0)
		exit(write(2, "Error: fork failed\n", 18));
	pid2 = fork();
	if (pid2 == 0)
		second_child(pipefd, av, env);
	else if (pid2 < 0)
		exit(write(2, "Error: fork failed\n", 18));
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status2))
		exit(WEXITSTATUS(status2));
	exit(1);
}
