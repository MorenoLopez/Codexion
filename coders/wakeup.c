/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wakeup.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: horarivo <horarivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 14:33:30 by horarivo          #+#    #+#             */
/*   Updated: 2026/09/02 14:47:14 by horarivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	wake_all_dongles(t_data *data)
{
	int	i;
	int	n;

	n = data->params->number_of_coders;
	i = 0;
	while (i < n)
	{
		pthread_mutex_lock(&data->dongles[i].dongle_mutex);
		pthread_cond_broadcast(&data->dongles[i].dongle_cond);
		pthread_mutex_unlock(&data->dongles[i].dongle_mutex);
		i++;
	}
}
