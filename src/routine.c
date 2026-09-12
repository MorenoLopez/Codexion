/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: horarivo <horarivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 11:12:54 by horarivo          #+#    #+#             */
/*   Updated: 2026/09/12 23:37:52 by horarivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	do_compile(t_coder *coder)
{
	if (!acquire_dongles(coder))
		return (0);
	coder->state = COMPILING;
	log_state(coder->data, coder->id_coder, "is compiling");
	pthread_mutex_lock(&coder->data->state_mutex);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&coder->data->state_mutex);
	usleep(coder->data->params->time_to_compile * 1000);
	release_dongle(coder->l_dongle);
	release_dongle(coder->r_dongle);
	pthread_mutex_lock(&coder->data->state_mutex);
	coder->compiles_done++;
	pthread_mutex_unlock(&coder->data->state_mutex);
	return (1);
}

void	do_debug(t_coder *coder)
{
	coder->state = DEBUGGING;
	log_state(coder->data, coder->id_coder, "is debugging");
	usleep(coder->data->params->time_to_debug * 1000);
}

void	do_refactor(t_coder *coder)
{
	coder->state = REFACTORING;
	log_state(coder->data, coder->id_coder, "is refactoring");
	usleep(coder->data->params->time_to_refactor * 1000);
}

void	*routine(void *arg)
{
	t_coder	*coder;
	int		n;
	int		stop;
	int		done;

	coder = (t_coder *)arg;
	n = coder->data->params->number_of_compiles_required;
	while (1)
	{
		pthread_mutex_lock(&coder->data->state_mutex);
		stop = coder->data->stop;
		done = coder->compiles_done;
		pthread_mutex_unlock(&coder->data->state_mutex);
		if (done == n || stop == 1)
			break ;
		if (!do_compile(coder))
			return (NULL);
		do_debug(coder);
		do_refactor(coder);
	}
	return (NULL);
}
