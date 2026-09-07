/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: horarivo <horarivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 22:33:47 by horarivo          #+#    #+#             */
/*   Updated: 2026/09/07 10:18:25 by horarivo         ###   ########.fr       */
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
	if (atoi(argv[1]) == 1)
		log_state(&data, data.coders[0].id_coder, "burned out");
	else
		start_simulation(&data);
	printf("\n");
	cleanup(&data);
	return (0);
}
