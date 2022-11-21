/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_other_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 07:02:08 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/21 18:27:40 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	p_util_destroy_mutexes(t_data *data, t_philo **philos)
{
	int	i;

	i = -1;
	while (++i < (int)data->philos_total)
	{
		pthread_mutex_destroy(&data->locks_forks[i]);
		pthread_mutex_destroy(&philos[i]->lock_meal_time);
	}
	pthread_mutex_destroy(&data->lock_log);
	pthread_mutex_destroy(&data->lock_sim_stop);
}

void	p_util_free_data(t_data **all_data)
{
	int		i;
	t_data	*data;

	data = *all_data;
	i = -1;
	if (!data)
		return ;
	if (data->locks_forks)
		free(data->locks_forks);
	free(data);
	*all_data = NULL;
}

void	p_util_free_philos(t_philo **philos)
{
	int		i;
	t_data	*data;

	i = -1;
	data = philos[0]->data;
	while (philos && ++i < (int)data->philos_total)
		if (philos[i])
			free(philos[i]);
	free(philos);
}
