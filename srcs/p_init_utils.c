/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_init_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 13:14:49 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/13 22:36:44 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	p_init_philos(t_data *data, t_philo **philos)
{
	size_t	i;

	i = 0;
	*philos = (t_philo *)malloc(sizeof(t_philo) * data->philos_total);
	if (!*philos)
		return ;
	while (i < data->philos_total)
	{
		(*philos)[i].fork_left = i;
		(*philos)[i].fork_right = (i + 1) % data->philos_total;
		(*philos)[i].meals_count = 0;
		(*philos)[i].time_last_meal = 0;
		(*philos)[i].time_limit_to_eat = 0;
		(*philos)[i].position_num = i + 1;
		(*philos)[i].data = data;
		(*philos)[i].pid = 0;
		i++;
	}
}

static void	p_init_mutex_forks(t_data *data)
{
	size_t	i;

	data->mutex_forks = malloc(sizeof(pthread_mutex_t) * data->philos_total);
	i = 0;
	while (i < data->philos_total)
	{
		pthread_mutex_init(&data->mutex_forks[i], NULL);
		i++;
	}
}

void	p_init_mutexes(t_data *data)
{
	p_init_mutex_forks(data);
	// pthread_mutex_init(&data->mutex_routine_end, NULL);
	pthread_mutex_init(&data->mutex_simulation_end, NULL);
}