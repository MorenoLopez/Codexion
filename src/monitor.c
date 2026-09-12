/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: horarivo <horarivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 11:24:49 by horarivo          #+#    #+#             */
/*   Updated: 2026/09/03 08:52:31 by horarivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_burnout(t_data *data, int i)
{
	long long	elapsed;

	pthread_mutex_lock(&data->state_mutex);
	elapsed = get_time_ms() - data->coders[i].last_compile_start;
	pthread_mutex_unlock(&data->state_mutex);
	if (elapsed > data->params->time_to_burnout)
	{
		log_state(data, data->coders[i].id_coder, "burned out");
		pthread_mutex_lock(&data->state_mutex);
		data->stop = 1;
		pthread_mutex_unlock(&data->state_mutex);
		return (1);
	}
	return (0);
}

int	check_all_coders(t_data *data, int n, int required)
{
	int	i;
	int	all_done;
	int	done;

	all_done = 1;
	i = 0;
	while (i < n)
	{
		if (check_burnout(data, i))
			return (-1);
		pthread_mutex_lock(&data->state_mutex);
		done = data->coders[i].compiles_done;
		pthread_mutex_unlock(&data->state_mutex);
		if (done < required)
			all_done = 0;
		i++;
	}
	return (all_done);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		n;
	int		required;
	int		result;

	data = (t_data *)arg;
	n = data->params->number_of_coders;
	required = data->params->number_of_compiles_required;
	while (1)
	{
		result = check_all_coders(data, n, required);
		if (result != 0)
		{
			pthread_mutex_lock(&data->state_mutex);
			data->stop = 1;
			pthread_mutex_unlock(&data->state_mutex);
			wake_all_dongles(data);
			return (NULL);
		}
		usleep(5000);
	}
}
