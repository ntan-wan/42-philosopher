/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_routine_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 09:34:05 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/12/07 18:03:54 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

static void	*p_routine_philo_one(t_philo *philo)
{
	sem_wait(philo->data->sem_forks);
	usleep(philo->data->time_to_die * 1000);
	p_log_status(philo, DIED);
	sem_post(philo->data->sem_forks);
	return (NULL);
}

/* 
	philosopher with even id number will be delayed at first
	to avoid conflicts with odd id number of philosopher.
 */
static void	p_routine_philo_loop(t_philo *philo)
{
	if (philo->id % 2 != 0)
		usleep(philo->data->time_to_eat * 1000);
	pthread_create(&philo->death_check, NULL, p_monitor_death, philo);
	pthread_detach(philo->death_check);
	while (!p_monitor_sim_has_stopped(philo->data))
	{
		p_philo_get_forks(philo);
		p_philo_eats(philo);
		p_philo_release_forks(philo);
		if (p_monitor_philo_is_full(philo))
		{
			philo->is_full = true;
			sem_post(philo->data->sem_full);
		}
		p_philo_sleeps(philo);
		p_philo_thinks(philo);
	}
}

void	p_routine_philo(t_philo *philo)
{
	sem_wait(philo->sem_meal);
	philo->time_last_meal = philo->data->time_start;
	sem_post(philo->sem_meal);
	p_util_delay(philo->data->time_start);
	if (philo->data->philos_total == 1)
		p_routine_philo_one(philo);
	else
		p_routine_philo_loop(philo);
}
