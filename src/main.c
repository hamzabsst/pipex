/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 09:32:30 by hbousset          #+#    #+#             */
/*   Updated: 2025/02/03 09:02:43 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

int	main(int ac, char **av, char **env)
{
	if (ac != 5)
		(ft_putstr_fd("Error: Invalid number of arguments\n", 2), exit(127));
	pipex(av, env);
	return (EXIT_SUCCESS);
}
