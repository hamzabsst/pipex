/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 08:49:07 by hbousset          #+#    #+#             */
/*   Updated: 2025/01/28 11:03:31 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../mylib/myLib.h"
# include <sys/wait.h>

void	pipex(char **av, char **env);
char	*get_command_path(char *cmd, char **env);
void	*ft_free(char **str);

#endif
