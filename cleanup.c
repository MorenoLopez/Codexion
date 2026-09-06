/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: horarivo <horarivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 08:36:23 by horarivo          #+#    #+#             */
/*   Updated: 2026/09/03 08:50:14 by horarivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	cleanup(t_data *data)
{
	int	i;

	if (!data)
		return ;
	i = 0;
	while (i < data->params->number_of_coders)
	{
		pthread_cond_destroy(&data->dongles[i].dongle_cond);
		pthread_mutex_destroy(&data->dongles[i].dongle_mutex);
		free(data->dongles[i].wait_queue.nodes);
		i++;
	}
	pthread_mutex_destroy(&data->log_mutex);
	pthread_mutex_destroy(&data->state_mutex);
	if (data->dongles)
	{
		free(data->dongles);
		data->dongles = NULL;
	}
	if (data->coders)
	{
		free(data->coders);
		data->coders = NULL;
	}
}
