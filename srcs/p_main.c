/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 11:06:25 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/08 11:43:59 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	p_data_clear_all(t_data *data)
{
	size_t	i;

	i = 0;
	while (i < data->philo_total)
	{
		pthread_mutex_destroy(&data->forks_mutex[i]);
		i++;
	}
	pthread_mutex_destroy(&data->write_mutex);
	pthread_mutex_destroy(&data->die_mutex);
	free(data->forks);
	free(data->philos);
	free(data->forks_mutex);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (p_is_invalid_input(ac, av))
		return (ERROR);
	p_input_parse(&data, av);
	p_init_philos(&data);
	p_init_forks(&data);
	p_init_mutexes(&data);
	p_routine_start(&data);
	pthread_mutex_lock(&data.die_mutex);
	pthread_mutex_unlock(&data.die_mutex);
	p_data_clear_all(&data);
	return (0);
}
