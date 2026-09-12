/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: horarivo <horarivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 10:09:27 by horarivo          #+#    #+#             */
/*   Updated: 2026/09/12 23:03:01 by horarivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_state(t_data *data, int id, char *msg)
{
	pthread_mutex_lock(&data->log_mutex);
	if (get_stop(data) == 1)
	{
		pthread_mutex_unlock(&data->log_mutex);
		return ;
	}
	printf("[ %lld ] %d %s\n", get_time_ms() - data->start_time, id + 1, msg);
	pthread_mutex_unlock(&data->log_mutex);
}

long long	compute_key(t_coder *coder)
{
	t_data		*data;
	long long	key;

	data = coder->data;
	if (data->params->scheduler == 0)
		return (get_time_ms());
	pthread_mutex_lock(&data->state_mutex);
	key = coder->last_compile_start + data->params->time_to_burnout;
	pthread_mutex_unlock(&data->state_mutex);
	return (key);
}

int	get_stop(t_data *data)
{
	int	stop;

	pthread_mutex_lock(&data->state_mutex);
	stop = data->stop;
	pthread_mutex_unlock(&data->state_mutex);
	return (stop);
}

int	dongle_available(t_dongle *d, t_data *data)
{
	return (d->is_taken == 0
		&& get_time_ms() - d->last_release >= data->params->dongle_cooldown);
}

int	has_priority(t_wait_node a, t_wait_node b)

{
	if (a.key != b.key)
		return (a.key < b.key);
	return (a.coder_id > b.coder_id);
}
