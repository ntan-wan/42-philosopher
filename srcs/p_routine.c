/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_routine.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 18:30:00 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/13 22:25:11 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int	p_is_dead(t_philo *philo)
{
	size_t	current_time;
	
	current_time = p_util_get_time(philo->data);
	if (current_time > philo->time_limit_to_eat)
	{
		p_util_log(current_time, philo, DIED);
		pthread_mutex_unlock(&philo->data->mutex_simulation_end);
		return (DIED);
	}
	return (0);
}

/* 
	we are going to assume the philosphers had their 
	meal even before the routine start. Hence we update
	time_last_meal and time_limit_to_eat first.
 */
void	*p_routine_loop(void *philosopher)
{
	t_philo	*philo;

	philo = (t_philo *)philosopher;
	philo->time_last_meal = p_util_get_time(philo->data);
	philo->time_limit_to_eat = philo->time_last_meal + philo->data->time_to_die;
	while (1)
	{
		// pthread_mutex_lock(&philo->data->mutex_routine_end);
		if (p_is_dead(philo))
			break ;
		p_action_take_forks(philo);
		p_action_eat(philo);
		p_action_sleep(philo);
		p_action_thinking(philo);
		// pthread_mutex_unlock(&philo->data->mutex_routine_end);
		// usleep(1);
	}
	return (NULL);
}

/*
	usleep() is to delay the next thread's execution a bit.
*/
void	p_routine_start(t_data *data, t_philo *philos)
{
	size_t		i;
	
	i = 0;
	data->time_routine_start = p_util_get_millisecond();
	while (i < data->philos_total)
	{
		pthread_create(&philos[i].pid, NULL, p_routine_loop, philos + i);
		pthread_detach(philos[i].pid);
		usleep(50);
		i++;
	}
	// i = 0;
	// while (i < data->philos_total)
	// {
		// pthread_join(philos->pid, NULL);
		// i++;
	// }
}