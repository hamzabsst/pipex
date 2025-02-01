/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:40:10 by hbousset          #+#    #+#             */
/*   Updated: 2025/02/01 11:34:00 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "../mylib/myLib.h"
# include <sys/wait.h>
# include <stdio.h>

void	handle_pipes(int ac, char **av, char **env);
void	execute_command_bonus(char *cmd, char **env);

#endif
