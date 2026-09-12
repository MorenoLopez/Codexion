/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: horarivo <horarivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 22:34:00 by horarivo          #+#    #+#             */
/*   Updated: 2026/09/03 08:49:50 by horarivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_dongles(t_data *data, int n)
{
	int	i;
	int	cooldown;

	i = 0;
	cooldown = data->params->dongle_cooldown;
	while (i < n)
	{
		pthread_cond_init(&data->dongles[i].dongle_cond, NULL);
		data->dongles[i].wait_queue.size = 0;
		data->dongles[i].wait_queue.capacity = n;
		data->dongles[i].wait_queue.nodes = malloc(sizeof(t_wait_node) * n);
		if (!data->dongles[i].wait_queue.nodes)
			return (1);
		data->dongles[i].id_dongle = i;
		data->dongles[i].is_taken = 0;
		pthread_mutex_init(&data->dongles[i].dongle_mutex, NULL);
		data->dongles[i].last_release = data->start_time - cooldown;
		i++;
	}
	return (0);
}

int	init_coders(t_data *data, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		data->coders[i].id_coder = i;
		data->coders[i].state = WAITING;
		data->coders[i].compiles_done = 0;
		data->coders[i].data = data;
		data->coders[i].r_dongle = &data->dongles[i];
		data->coders[i].l_dongle = &data->dongles[(i - 1 + n) % n];
		data->coders[i].last_compile_start = data->start_time;
		i++;
	}
	return (0);
}

int	init_data(t_data *data)
{
	int	n;

	pthread_mutex_init(&data->log_mutex, NULL);
	pthread_mutex_init(&data->state_mutex, NULL);
	data->stop = 0;
	n = data->params->number_of_coders;
	data->start_time = get_time_ms();
	data->coders = malloc(sizeof(t_coder) * n);
	data->dongles = malloc(sizeof(t_dongle) * n);
	if (!data->coders || !data->dongles)
		return (1);
	if (init_dongles(data, n) != 0)
		return (1);
	init_coders(data, n);
	return (0);
}
