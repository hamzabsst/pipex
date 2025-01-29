/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 09:32:30 by hbousset          #+#    #+#             */
/*   Updated: 2025/01/28 11:12:32 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../inc/pipex.h"

int	main(int ac, char **av, char **env)
{
	if (ac != 5)
		return (write(2, "Error: invalid number of arguments\n", 36));
	if (!av[2] || !*av[2] || !av[3] || !*av[3])
		return (write(2, "Error: empty command\n", 20));
	pipex(av, env);
	return (0);
}
