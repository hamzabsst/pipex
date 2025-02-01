/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:40:10 by hbousset          #+#    #+#             */
/*   Updated: 2025/02/01 19:55:00 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "../mylib/myLib.h"
# include <sys/wait.h>
# include <stdio.h>

typedef struct s_pipex
{
	int		cmd_count;
	int		**pipes;
	pid_t	*pids;
	char	**av;
	char	**env;
	int		ac;
}	t_pipex;

void	handle_pipes(int ac, char **av, char **env);
void	execute_command_bonus(char *cmd, char **env, t_pipex *px);
void	free_pipes(t_pipex *px);
void	close_pipes(t_pipex *px);
void	cleanup(t_pipex *px);
void	init_first_child(t_pipex *px);
void	mid_child(int i, t_pipex *px);
void	last_child(t_pipex *px);

#endif
