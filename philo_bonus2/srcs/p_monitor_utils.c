/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_monitor_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 14:52:32 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/12/06 19:42:08 by ntan-wan         ###   ########.fr       */
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
	return (data->meals_min && philo->meals_count >= data->meals_min);
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
		time_passed = time_current - philo->time_last_meal;
		if (time_passed > philo->data->time_to_die)
		{
			p_log_status(philo, DIED);
			sem_wait(philo->data->sem_log);
			p_log_death_report(time_current, philo);
			p_monitor_set_sim_stop(philo->data, true);
			sem_post(philo->sem_meal);
			break ;
		}
		usleep(ONE_MS);
	}
	exit(PHILO_IS_DEAD);
	return (NULL);
}
