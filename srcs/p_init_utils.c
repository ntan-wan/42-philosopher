/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_init_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:17:18 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/21 11:36:21 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	p_init_data_parsing(t_data *data, char **av)
{
	if (av[5])
		data->meals_minimum = p_util_atoi(av[5]);
	else
		data->meals_minimum = 0;
	data->philos_total = p_util_atoi(av[1]);
	data->time_to_die = p_util_atoi(av[2]);
	data->time_to_eat = p_util_atoi(av[3]);
	data->time_to_sleep = p_util_atoi(av[4]);
}

static void	p_assign_forks(t_philo *philo)
{
	if (philo->id % 2 != 0)
	{
		philo->fork[0] = (philo->id + 1) % philo->data->philos_total;
		philo->fork[1] = philo->id;
	}
	else
	{
		philo->fork[0] = philo->id;
		philo->fork[1] = (philo->id + 1) % philo->data->philos_total;
	}
}

static int	p_init_philos(t_data *data)
{
	unsigned int	i;
	t_philo			**philos;

	i = 0;
	philos = (t_philo **)malloc(sizeof(t_philo *) * data->philos_total);
	data->philos = philos;
	if (!philos)
		return (p_util_error_print(ERR_MALLOC));
	while (i < data->philos_total)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (p_util_error_print(ERR_MALLOC));
		else if (pthread_mutex_init(&philos[i]->lock_meal_time, NULL) != 0)
			return (p_util_error_print(ERR_MUTEX));
		philos[i]->id = i;
		philos[i]->thread = 0;
		philos[i]->data = data;
		philos[i]->meals_count = 0;
		p_assign_forks(philos[i]);
		i++;
	}
	data->sim_stop = false;
	return (SUCCESS);
}

static int	p_init_global_mutexes(t_data *data)
{
	unsigned int	i;

	i = 0;
	data->locks_forks = malloc(sizeof(pthread_mutex_t) * data->philos_total);
	if (!data->locks_forks)
		return (p_util_error_print(ERR_MALLOC));
	else if (pthread_mutex_init(&data->lock_sim_stop, NULL) != 0)
		return (p_util_error_print(ERR_MUTEX));
	else if (pthread_mutex_init(&data->lock_log, NULL) != 0)
		return (p_util_error_print(ERR_MUTEX));
	while (i < data->philos_total)
		if (pthread_mutex_init(&(data->locks_forks[i++]), NULL) != 0)
			return (p_util_error_print(ERR_MUTEX));
	return (SUCCESS);
}

t_data	*p_init_data(char **av)
{
	t_data	*data;

	data = malloc(sizeof(t_data) * 1);
	if (!data)
		return (NULL);
	p_init_data_parsing(data, av);
	p_init_philos(data);
	p_init_global_mutexes(data);
	return (data);
}
