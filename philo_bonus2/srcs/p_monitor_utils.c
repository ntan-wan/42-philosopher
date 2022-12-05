/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_monitor_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 14:52:32 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/12/05 15:45:07 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"
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

static bool	p_philo_died(t_philo *philo)
{
	time_t	time_current;
	time_t	time_passed;

	time_current = p_util_get_time_in_ms();
	time_passed = time_current - philo->time_last_meal;
	if (time_passed > philo->data->time_to_die)
	{
		p_log_status(philo, DIED);
		// p_log_death_report(time_current, philo);
		p_monitor_set_sim_stop(philo->data, true);
		// p_kill_all_philos(philo->data);
		sem_post(philo->sem_meal);
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
		sem_wait(philos[i]->sem_meal);
		if (p_philo_died(philos[i]))
			return (true);
		if (data->meals_minimum
			&& philos[i]->meals_count < data->meals_minimum)
			all_ate_enough = false;
		sem_post(philos[i]->sem_meal);
	}
	if (data->meals_minimum && all_ate_enough)
	{
		p_monitor_set_sim_stop(data, true);
		return (true);
	}
	return (false);
}

void	test(t_philo **philos)
{
	int	i;
	t_data	*data;

	i = 0;
	data = philos[0]->data;
	while (i < data->philos_total)
		if (philos[i]->time_last_meal)
			i++;
}

void	*p_monitor_philo(void *philosophers)
{
	t_philo	**philos;

	philos = (t_philo **)philosophers;
	// printf("%d\n", philos[0]->data->philos_total);
	p_util_delay(philos[0]->data->time_start);
	// test(philos);
	// printf("%ld\n", philos[0]->time_last_meal);
	while (1)
	{
		if (p_monitor_has_ended(philos))
			break ;
		usleep(ONE_MS);
	}
	return (NULL);
}
