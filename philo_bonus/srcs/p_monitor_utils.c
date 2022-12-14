/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_monitor_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 14:52:32 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/12/14 16:25:10 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"
#define ONE_MS 1000

void	p_monitor_set_sim_stop(t_data *data, bool state)
{
	sem_wait(data->sem_sim_stop);
	data->sim_stop = state;
	sem_post(data->sem_sim_stop);
}

bool	p_monitor_sim_has_stopped(t_data *data)
{
	bool	result;

	sem_wait(data->sem_sim_stop);
	result = data->sim_stop;
	sem_post(data->sem_sim_stop);
	return (result);
}

int	p_monitor_philo_is_full(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	return (!philo->is_full
		&& data->meals_min && philo->meals_count >= data->meals_min);
}

void	*p_monitor_death(void *philosopher)
{
	t_philo	*philo;
	time_t	time_current;
	time_t	time_passed;

	philo = (t_philo *)philosopher;
	p_util_delay(philo->data->time_start);
	while (1)
	{
		time_current = p_util_get_time_in_ms();
		sem_wait(philo->sem_meal);
		time_passed = time_current - philo->time_last_meal;
		sem_post(philo->sem_meal);
		if (time_passed > philo->data->time_to_die)
		{
			p_log_status(philo, DIED);
			sem_wait(philo->data->sem_log);
			p_monitor_set_sim_stop(philo->data, true);
			break ;
		}
		usleep(ONE_MS);
	}
	exit(PHILO_IS_DEAD);
	return (NULL);
}

void	*p_monitor_is_full_check(void *all_data)
{
	t_data			*data;
	unsigned int	philo_is_full;

	philo_is_full = 0;
	data = (t_data *)all_data;
	while (data->meals_min && philo_is_full != data->philos_total)
	{
		sem_wait(data->sem_full);
		philo_is_full++;
	}
	if (philo_is_full == data->philos_total)
		p_util_kill_all_philos(data);
	return (NULL);
}
