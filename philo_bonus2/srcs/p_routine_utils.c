/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_routine_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 09:34:05 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/12/05 16:24:46 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	*p_routine_philo_one(t_philo *philo)
{
	sem_wait(philo->data->sem_forks);
	usleep(philo->data->time_to_die * 1000);
	p_log_status(philo, DIED);
	sem_post(philo->data->sem_forks);
	return (NULL);
}

void p_kill_all_philos(t_data *data)
{
	int	i;

	i = -1;
	while (++i < (int)data->philos_total)
		kill(data->pids[i], SIGKILL);
}

void	*p_death_check(void *philosopher)
{
	t_philo	*philo;
	time_t	time_current;
	time_t	time_passed;

	philo = (t_philo *)philosopher;
	//
	while (1)
	{
		time_current = p_util_get_time_in_ms();
		time_passed = time_current - philo->time_last_meal;
		if (time_passed > philo->data->time_to_die)
		{
			p_log_status(philo, DIED);
			// p_log_death_report(time_current, philo);
			p_monitor_set_sim_stop(philo->data, true);
			// p_kill_all_philos(philo->data);
			sem_post(philo->sem_meal);
			break ;
		}
	}
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
	pthread_create(&philo->death_check, NULL, p_death_check, philo);
	pthread_detach(philo->death_check);
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
	sem_wait(philo->sem_meal);
	philo->time_last_meal = philo->data->time_start;
	sem_post(philo->sem_meal);
	p_util_delay(philo->data->time_start);
	if (philo->data->philos_total == 1)
		return (p_routine_philo_one(philo));
	else
		p_routine_philo_loop(philo);
	return (NULL);
}
