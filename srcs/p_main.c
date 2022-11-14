/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 10:38:04 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/14 17:32:56 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	p_data_parsing(t_data *data, char **av)
{
	if (!data || !av)
		return ;
	if (av[5])
		data->meals_minimum = p_util_a_to_unsigned_int(av[5]);
	else
		data->meals_minimum = 0;
	data->philos_total = p_util_a_to_unsigned_int(av[1]);
	data->time_to_die = p_util_a_to_unsigned_int(av[2]);
	data->time_to_eat = p_util_a_to_unsigned_int(av[3]);
	data->time_to_sleep = p_util_a_to_unsigned_int(av[4]);
	data->time_routine_start = 0;
	data->philos_finished_count = 0;
}

static void	p_clear_all(t_data *data, t_philo *philos)
{
	size_t	i;

	i = 0;
	while (i < data->philos_total)
		pthread_mutex_destroy(&data->mutex_forks[i++]);
	pthread_mutex_destroy(&data->mutex_die);
	pthread_mutex_destroy(&data->mutex_simulation_end);
	free(philos);
	free(data->mutex_forks);
}

int main(int ac, char **av)
{
	t_data		data;
	t_philo		*philos;

	philos = NULL;
	if (is_invalid_input(ac, av))
		return (ERROR);
	p_data_parsing(&data, av);
	p_init_philos(&data, &philos);
	p_init_mutexes(&data);
	pthread_mutex_lock(&data.mutex_simulation_end);
	p_routine_start(&data, philos);
	pthread_mutex_lock(&data.mutex_simulation_end);
	p_clear_all(&data, philos);
	return (0);
}