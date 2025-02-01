/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:40:12 by hbousset          #+#    #+#             */
/*   Updated: 2025/02/01 09:43:31 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"pipex_bonus.h"

int	check_syntax(int ac, char **av, char **env)
{
	if (!ft_strcmp("here_doc", av[1]))
	{
		if (ac != 6)
			(ft_putstr_fd("invalid structure\n", 2), exit(127));
		av[2] = ft_strjoin(av[2], "\n");
		return (1);
	}
	return (0);
}
