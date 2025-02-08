/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:40:10 by hbousset          #+#    #+#             */
/*   Updated: 2025/02/08 11:16:04 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../mylib/myLib.h"
# include <sys/wait.h>
# include <stdio.h>

# define CLOSE_PIPES 1
# define FULL_CLEANUP 2
# define FREE_PIPES 3
# define HEREDOC_CLEANUP 4

typedef struct s_pipex
{
	int		cmd_count;
	int		**pipes;
	pid_t	*pids;
	char	**av;
	char	**env;
	int		ac;
	char	**cmd_args;
	char	*cmd_path;
	char	**paths;
	int		curr_cmd;
	int		mode;
}	t_pipex;

void	cleanup(t_pipex *px);
void	execute_command(t_pipex *px);
void	here_doc(char **av, char **env);
void	create_processes(t_pipex *px, char *tmp);
void	create_child(t_pipex *px);
int		wait_for_children(t_pipex *px);
char	**ft_split_quote(const char *s, char c);
void	*free_split(char **str, int index);

#endif
