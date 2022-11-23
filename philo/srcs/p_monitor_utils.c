/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_monitor_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 14:52:32 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/23 19:41:18 by ntan-wan         ###   ########.fr       */
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
	// printf("philo %d ,time current m-> %ld\n", philo->id, time_current);
	// printf("time passed m-> %ld\n", time_passed);
	// printf("time last meal m-> %ld\n", philo->time_last_meal);;
	if (time_passed > philo->data->time_to_die)
	{
		p_log_status(philo, DIED);
		// printf("time pased -> %ld\n", time_passed);
		p_log_death_report(time_current, philo);
		p_monitor_set_sim_stop(philo->data, true);
		pthread_mutex_unlock(&philo->lock_meal_time);
		return (true);
	}
	return (false);
}

static bool	p_monitor_has_ended(t_philo **philos)
{
	int		i;
	t_data	*data;
	bool	all_ate_enough;

	i = -1;
	all_ate_enough = true;
	data = philos[0]->data;
	while (++i < (int)data->philos_total)
	{
		pthread_mutex_lock(&philos[i]->lock_meal_time);
		if (p_philo_died(philos[i]))
			return (true);
		if (data->meals_minimum
			&& philos[i]->meals_count < data->meals_minimum)
			all_ate_enough = false;
		pthread_mutex_unlock(&philos[i]->lock_meal_time);
	}
	if (data->meals_minimum && all_ate_enough)
	{
		p_monitor_set_sim_stop(data, true);
		return (true);
	}
	return (false);
}

void	p_monitor_delay(t_philo **philos)
{
	unsigned int	i;
	t_data			*data;

	i = 0;
	data = philos[0]->data;
	while (i < data->philos_total)
		if (philos[i]->time_last_meal)
			i++;
}

void	*p_monitor_philo(void *philosophers)
{
	// t_data	*data;
	t_philo	**philos;

	philos = (t_philo **)philosophers;
	// data = philos[0]->data;
	// p_util_delay(data->time_start);
	// time_t	before;
	// time_t	after;
	// before = p_util_get_time_in_ms();
	// printf("before -> %ld\n", before);
	p_monitor_delay(philos);
	// after = p_util_get_time_in_ms();
	// printf("after -> %ld\n", after);
	// printf("diff -> %ld\n", after - before);
	while (1)
	{
		if (p_monitor_has_ended(philos))
			break ;
		usleep(ONE_MS);
	}
	return (NULL);
}

bool	p_monitor_sim_has_stopped(t_data *data)
{
	bool	result;

	pthread_mutex_lock(&data->lock_sim_stop);
	result = data->sim_stop;
	pthread_mutex_unlock(&data->lock_sim_stop);
	return (result);
}
