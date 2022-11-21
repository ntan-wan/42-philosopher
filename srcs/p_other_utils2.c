/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_other_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 07:02:08 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/21 15:14:28 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	p_util_destroy_mutexes(t_data *data, t_philo **philos)
{
	unsigned int	i;

	i = 0;
	while (i < data->philos_total)
	{
		pthread_mutex_destroy(&data->locks_forks[i]);
		pthread_mutex_destroy(&philos[i]->lock_meal_time);
		i++;
	}
	pthread_mutex_destroy(&data->lock_log);
	pthread_mutex_destroy(&data->lock_sim_stop);
}

void	p_util_free_data_and_philos(t_data **all_data, t_philo **philos)
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
	while (philos && i < data->philos_total)
	{
		if (philos[i])
			free(philos[i]);
		i++;
	}
	free(philos);
	free(data);
	*all_data = NULL;
}