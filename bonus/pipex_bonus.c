/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:40:08 by hbousset          #+#    #+#             */
/*   Updated: 2025/02/01 09:33:42 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"pipex_bonus.h"

int	main(int ac, char **av, char **env)
{
	if (ac < 5)
	{
		ft_putstr_fd("Error: Invalid number of arguments\n", 2);
		exit(127);
	}
	if (check_syntax(ac, av, env))
		handle_pipes(ac, av, env);
	else
		here_doc(ac, av, env);
}
