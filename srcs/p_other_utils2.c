/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_other_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 07:02:08 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/21 07:54:19 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	p_util_destroy_mutexes(t_data *data)
{
	unsigned int	i;

	i = 0;
	while (i < data->philos_total)
	{
		pthread_mutex_destroy(&data->locks_forks[i]);
		pthread_mutex_destroy(&data->philos[i]->lock_meal_time);
		i++;
	}
	pthread_mutex_destroy(&data->lock_log);
	pthread_mutex_destroy(&data->lock_sim_stop);
}

void	p_util_free_data(t_data **all_data)
{
	unsigned int	i;
	t_data 			*data;
	
	data = *all_data;
	i = 0;
	if (!data)	
		return ;
	if (data->locks_forks)
	{
		free(data->locks_forks);
		data->locks_forks = NULL;
	}
	while (data->philos && i < data->philos_total)
	{
		if (data->philos[i])
			free(data->philos[i]);
		i++;
	}
	free(data->philos);
	data->philos = NULL;
	free(data);
	*all_data = NULL;
}