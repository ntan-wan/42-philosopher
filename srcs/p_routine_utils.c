/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_routine_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 09:34:05 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/21 19:57:39 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	*p_routine_philo_one(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->locks_forks[philo->fork[0]]);
	p_util_usleep(philo->data, philo->data->time_to_die);
	p_log_status(philo, DIED);
	pthread_mutex_unlock(&philo->data->locks_forks[philo->fork[0]]);
	return (NULL);
}

/* 
	philosopher with even id number start their routine by "thinking"
	to avoid conflicts with odd id number of philosopher.
 */
static void	p_routine_philo_loop(t_philo *philo)
{
	if (philo->id % 2 != 0)
	{
		p_log_status(philo, THINKING);
		usleep(philo->data->time_to_eat * 1000);
	}
	while (!p_monitor_sim_has_stopped(philo->data))
	{
		p_philo_get_forks(philo);
		p_philo_eats(philo);
		p_philo_release_forks(philo);
		p_philo_sleeps(philo);
		p_philo_thinks(philo);
	}
}

void	*p_routine_philo(void *philosopher)
{
	t_philo	*philo;

	philo = (t_philo *)philosopher;
	pthread_mutex_lock(&philo->lock_meal_time);
	philo->time_last_meal = philo->data->time_start;
	pthread_mutex_unlock(&philo->lock_meal_time);
	p_util_delay(philo->data->time_start);
	if (philo->data->philos_total == 1)
		return (p_routine_philo_one(philo));
	else
		p_routine_philo_loop(philo);
	return (NULL);
}
