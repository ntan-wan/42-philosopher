/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_init_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 06:50:42 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/08 11:04:26 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	p_init_mutexes(t_data *data)
{
	size_t	i;

	data->forks_mutex = malloc(sizeof(pthread_mutex_t) * data->philo_total);
	if (!data->forks_mutex)
		return ;
	i = 0;
	while (++i < data->philo_total)
	{
		pthread_mutex_init(&data->forks_mutex[i], NULL);
		i++;
	}
	pthread_mutex_init(&data->write_mutex, NULL);
	pthread_mutex_init(&data->die_mutex, NULL);
}

void	p_init_forks(t_data *data)
{
	size_t	i;

	i = 0;
	data->forks = malloc(sizeof(int) * data->philo_total);
	while (i < data->philo_total)
	{
		data->forks[i] = 0;
		i++;
	}
}

void	p_init_philos(t_data *data)
{
	size_t	i;
	
	data->philos = malloc(sizeof(t_philo) * data->philo_total);
	if (!data->philos)
		return ;
	i = 0;
	while (i < data->philo_total)
	{
		data->philos[i].position = i;
		data->philos[i].left_fork = i;
		data->philos[i].right_fork = (i + 1) % data->philo_total;
		data->philos[i].meals = 0;
		data->philos[i].eating = 0;
		data->philos[i].base_data = data;
		i++;
	}
}
