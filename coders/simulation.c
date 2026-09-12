/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: horarivo <horarivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 10:09:15 by horarivo          #+#    #+#             */
/*   Updated: 2026/09/01 11:39:41 by horarivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	start_simulation(t_data *data)
{
	int	i;
	int	n;

	n = data->params->number_of_coders;
	pthread_create(&data->monitor_thread, NULL, monitor_routine, data);
	i = 0;
	while (i < n)
	{
		pthread_create(&data->coders[i].coder_thread,
			NULL, routine, &data->coders[i]);
		i++;
	}
	i = 0;
	while (i < n)
	{
		pthread_join(data->coders[i].coder_thread, NULL);
		i++;
	}
	pthread_join(data->monitor_thread, NULL);
	return (0);
}
