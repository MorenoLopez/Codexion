/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: horarivo <horarivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 22:34:04 by horarivo          #+#    #+#             */
/*   Updated: 2026/09/01 10:09:42 by horarivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_num(char *str)
{
	int	i;

	if (str[0] == '\0' || strlen(str) > 10)
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	set_and_check(t_sim *sim, char **argv)
{
	sim->number_of_coders = atoi(argv[1]);
	sim->time_to_burnout = atoi(argv[2]);
	sim->time_to_compile = atoi(argv[3]);
	sim->time_to_debug = atoi(argv[4]);
	sim->time_to_refactor = atoi(argv[5]);
	sim->number_of_compiles_required = atoi(argv[6]);
	sim->dongle_cooldown = atoi(argv[7]);
	if (sim->number_of_coders <= 0 || sim->time_to_burnout <= 0
		|| sim->time_to_compile <= 0 || sim->time_to_debug <= 0
		|| sim->time_to_refactor <= 0
		|| sim->number_of_compiles_required <= 0
		|| sim->dongle_cooldown < 0)
		return (1);
	return (0);
}

int	parser(t_sim *sim, char **argv)
{
	int	i;

	i = 1;
	while (i <= 7)
	{
		if (!is_num(argv[i]))
			return (1);
		i++;
	}
	if (set_and_check(sim, argv) != 0)
		return (1);
	if (strcmp(argv[8], "fifo") == 0)
		sim->scheduler = 0;
	else if (strcmp(argv[8], "edf") == 0)
		sim->scheduler = 1;
	else
		return (1);
	return (0);
}
