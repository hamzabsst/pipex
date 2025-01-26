/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 09:18:35 by hbousset          #+#    #+#             */
/*   Updated: 2025/01/26 10:08:42 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

int	check_errors(int ac, char **av)
{
	int	i;
	if (ac != 4)
		return(write(2, "invalid number of arguments\n", 28));
}
