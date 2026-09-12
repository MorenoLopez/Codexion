/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: horarivo <horarivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 10:06:06 by horarivo          #+#    #+#             */
/*   Updated: 2026/09/03 09:01:20 by horarivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	acquire_normal(t_coder *coder)
{
	if (!take_dongle(coder, coder->l_dongle))
		return (0);
	log_state(coder->data, coder->id_coder, "has taken a dongle");
	if (!take_dongle(coder, coder->r_dongle))
	{
		release_dongle(coder->l_dongle);
		return (0);
	}
	log_state(coder->data, coder->id_coder, "has taken a dongle");
	return (1);
}

int	acquire_reversed(t_coder *coder)
{
	if (!take_dongle(coder, coder->r_dongle))
		return (0);
	log_state(coder->data, coder->id_coder, "has taken a dongle");
	if (!take_dongle(coder, coder->l_dongle))
	{
		release_dongle(coder->r_dongle);
		return (0);
	}
	log_state(coder->data, coder->id_coder, "has taken a dongle");
	return (1);
}

int	acquire_dongles(t_coder *coder)
{
	int	last;

	last = (coder->id_coder == coder->data->params->number_of_coders - 1);
	if (last)
		return (acquire_reversed(coder));
	return (acquire_normal(coder));
}

int	take_dongle(t_coder *coder, t_dongle *d)
{
	t_data			*data;
	struct timespec	ts;

	data = coder->data;
	pthread_mutex_lock(&d->dongle_mutex);
	heap_push(&d->wait_queue, compute_key(coder), coder->id_coder);
	while (get_stop(data) == 0 && (!dongle_available(d, data)
			|| heap_peek(&d->wait_queue) != coder->id_coder))
	{
		build_timeout(&ts);
		pthread_cond_timedwait(&d->dongle_cond, &d->dongle_mutex, &ts);
	}
	if (get_stop(data) == 1)
	{
		heap_remove(&d->wait_queue, coder->id_coder);
		pthread_mutex_unlock(&d->dongle_mutex);
		return (0);
	}
	heap_pop(&d->wait_queue);
	d->is_taken = 1;
	pthread_mutex_unlock(&d->dongle_mutex);
	return (1);
}

void	release_dongle(t_dongle *d)
{
	pthread_mutex_lock(&d->dongle_mutex);
	d->is_taken = 0;
	d->last_release = get_time_ms();
	pthread_cond_broadcast(&d->dongle_cond);
	pthread_mutex_unlock(&d->dongle_mutex);
}
