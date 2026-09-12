/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: horarivo <horarivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 22:33:47 by horarivo          #+#    #+#             */
/*   Updated: 2026/09/12 23:34:53 by horarivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_sim	arguments;
	t_data	data;

	if ((argc != 9) || (parser(&arguments, argv) != 0))
	{
		fprintf(stderr, "Invalid arguments\n");
		return (1);
	}
	data.params = &arguments;
	if (init_data(&data) != 0)
	{
		fprintf(stderr, "init_data failed\n");
		return (1);
	}
	start_simulation(&data);
	printf("\n");
	cleanup(&data);
	return (0);
}
