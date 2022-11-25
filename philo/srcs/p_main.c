/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:05:16 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/25 21:21:02 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int	p_monitor_start(pthread_t *thread_monitor, t_philo **philos)
{
	if (philos[0]->data->philos_total > 1)
	{
		if (pthread_create(thread_monitor, NULL,
				p_monitor_philo, philos) != 0)
			return (p_util_error_print(ERR_THREAD));
	}
	return (SUCCESS);
}

static int	p_sim_start(t_data *data, t_philo **philos)
{
	int		i;
	time_t	time_adjustment;

	i = -1;
	time_adjustment = data->philos_total * 2 * 10;
	data->time_start = p_util_get_time_in_ms() + time_adjustment;
	while (++i < (int)data->philos_total)
	{
		if (pthread_create(&philos[i]->thread, NULL,
				p_routine_philo, philos[i]) != 0)
			return (p_util_error_print(ERR_THREAD));
	}
	return (SUCCESS);
}

static void	p_sim_end(t_data **data, pthread_t thread_monitor, t_philo **philos)
{
	int	i;

	i = -1;
	while (++i < (int)(*data)->philos_total)
		pthread_join(philos[i]->thread, NULL);
	if ((*data)->philos_total > 1)
		pthread_join(thread_monitor, NULL);
	if ((*data)->meals_minimum)
		p_log_meals_report(philos);
	p_util_destroy_mutexes(*data, philos);
	p_util_free_philos(philos);
	p_util_free_data(data);
}

int	main(int ac, char **av)
{
	t_data		*data;
	t_philo		**philos;
	pthread_t	thread_monitor;

	data = NULL;
	if (is_invalid_input(ac, av))
		return (EXIT_FAILURE);
	data = p_init_data(av);
	philos = p_init_philos(data);
	if (!data || !philos)
		return (p_util_error_print(ERR_MALLOC));
	else if (p_sim_start(data, philos) == ERROR)
		return (EXIT_FAILURE);
	else if (p_monitor_start(&thread_monitor, philos) == ERROR)
		return (EXIT_FAILURE);
	p_sim_end(&data, thread_monitor, philos);
	return (EXIT_SUCCESS);
}
