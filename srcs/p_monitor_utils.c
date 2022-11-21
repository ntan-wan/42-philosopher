/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_monitor_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 14:52:32 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/21 15:07:53 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"
#define ONE_MS 1000

static void	p_monitor_set_sim_stop(t_data *data, bool state)
{
	pthread_mutex_lock(&data->lock_sim_stop);
	data->sim_stop = state;
	pthread_mutex_unlock(&data->lock_sim_stop);
}

static bool	p_philo_died(t_philo *philo)
{
	time_t	time_current;
	time_t	time_passed;

	time_current = p_util_get_time_in_ms();
	time_passed = time_current - philo->time_last_meal;
	if (time_passed >= philo->data->time_to_die)
	{
		p_log_status(philo, DIED);
		p_monitor_set_sim_stop(philo->data, true);
		pthread_mutex_unlock(&philo->lock_meal_time);
		return (true);
	}
	return (false);
}

static bool	p_monitor_has_ended(t_philo **philos)
{
	unsigned int	i;
	bool			all_ate_enough;
	t_data			*data;

	i = 0;
	all_ate_enough = true;
	data = philos[0]->data;
	while (i < data->philos_total)
	{
		pthread_mutex_lock(&philos[i]->lock_meal_time);
		if (p_philo_died(philos[i]))
			return (true);
		if (data->meals_minimum
			&& philos[i]->meals_count < data->meals_minimum)
			all_ate_enough = false;
		pthread_mutex_unlock(&philos[i]->lock_meal_time);
		i++;
	}
	if (data->meals_minimum && all_ate_enough)
	{
		p_monitor_set_sim_stop(data, true);
		return (true);
	}
	return (false);
}

static void	*p_monitor_philo(void *philosophers)
{
	t_data	*data;
	t_philo	**philos;

	philos = (t_philo **)philosophers;
	data = philos[0]->data;
	p_monitor_set_sim_stop(data, false);
	p_util_delay(data->time_start);
	while (1)
	{
		if (p_monitor_has_ended(philos))
			break ;
		usleep(ONE_MS);
	}
	return (NULL);
}

int	p_monitor_start(pthread_t *thread_monitor, t_philo **philos)
{
	if (philos[0]->data->philos_total > 1)
	{
		if (pthread_create(thread_monitor, NULL,
				p_monitor_philo, philos) != 0)
			return (p_util_error_print(ERR_THREAD));
	}
	return (SUCCESS);
}
