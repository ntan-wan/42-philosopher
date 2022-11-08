/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_routine.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 11:34:56 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/08 11:41:10 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	*p_routine_loop(void *philosopher)
{
	t_philo	*philo;

	philo = (t_philo *)philosopher;
	philo->last_meal = p_util_get_milisecond();
	philo->threshold = philo->last_meal + philo->base_data->time_to_die;
	while (1)
	{
		if (p_philo_take_forks(philo) || p_philo_eat(philo) || p_philo_release_forks(philo))
			break ;
	}
	return (NULL);
}

void	p_routine_start(t_data *data)
{
	size_t		i;
	pthread_t	tid;

	i = 0;
	pthread_mutex_lock(&data->die_mutex);
	while (i < data->philo_total)
	{
		pthread_create(&tid, NULL, p_routine_loop, (void *)&data->philos[i]);
		pthread_detach(tid);
		if (data->philo_total % 2)
			p_util_usleep(500);
		i++;
	}
}