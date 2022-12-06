/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_routine_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 09:34:05 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/12/06 17:15:53 by ntan-wan         ###   ########.fr       */
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

void p_kill_child_philos(t_data *data)
{
	int	i;

	i = -1;
	// while (++i < (int)data->philos_total - 1)
		// kill(data->pids[i], SIGINT);
	while (data->pids[++i])
		kill(data->pids[i], SIGINT);
}

/*void	*philo_exit(t_philo *philo)
{
	// pthread_detach(philo->death_check);
	p_util_close_global_semaphores(philo->data);
	sem_close(philo->sem_meal);
	free(philo->data->pids);
	free(philo->data);
	free(philo);
	exit(0);
	return (NULL);
}*/

void	*p_death_check(void *philosopher)
{
	t_philo	*philo;
	time_t	time_current;
	time_t	time_passed;

	philo = (t_philo *)philosopher;
	//
	// p_util_delay(philo->data->time_start);
	while (1)
	{
		time_current = p_util_get_time_in_ms();
		time_passed = time_current - philo->time_last_meal;
		if (time_passed > philo->data->time_to_die)
		{
			p_log_status(philo, DIED);
			// sem_wait(philo->data->sem_log);
			// p_log_death_report(time_current, philo);
			p_monitor_set_sim_stop(philo->data, true);
			sem_post(philo->sem_meal);
			break ;
		}
		usleep(1000);
	}
	exit(PHILO_IS_DEAD);
	return (NULL);
	// return (philo_exit(philo));
}

int	p_philo_is_full(t_philo *philo)
{
	t_data	*data;
	
	data = philo->data;
	return (data->meals_min && philo->meals_count >= data->meals_min);
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
		if (p_philo_is_full(philo))
			break ;
		p_philo_sleeps(philo);
		p_philo_thinks(philo);
	}
	exit(PHILO_IS_FULL);
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
