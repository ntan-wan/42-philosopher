/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_init_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:17:18 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/21 20:32:36 by ntan-wan         ###   ########.fr       */
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
	data->sim_stop = false;
}

/* 
	Philo with odd id will have their forks reversed.
	Without reverse:
	philo 0 -> fork 0 and fork 1
	philo 1 -> fork 1 and fork 2
	philo 2 -> fork 2 and fork 0
	If philo 0 takes fork 0, philo 1 takes fork 1, and philo 2 takes fork 2,
	there will be deadlock as everyone's second fork is still being used
	by other philos.
	with reverse :
	philo 0 -> fork 1 and fork 0
	philo 1 -> fork 1 and fork 2 
	philo 2 -> fork 0 and fork 2
	If philo 2 takes fork 0, philo 0 takes fork 1 and philo 1 waits patiently
	for fork 1, philo 2 is able to get fork 2 as no one has acquired yet.When
	philo 2 is done eating, philo 0 can acquires fork 0 and start eating.
	When philo 0 is done eating, philo 1 can then start eating.
 */
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

t_philo	**p_init_philos(t_data *data)
{
	int		i;
	t_philo	**philos;

	i = -1;
	philos = (t_philo **)malloc(sizeof(t_philo *) * data->philos_total);
	if (!philos)
		return (NULL);
	while (++i < (int)data->philos_total)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (NULL);
		else if (pthread_mutex_init(&philos[i]->lock_meal_time, NULL)
			!= SUCCESS)
			return (NULL);
		philos[i]->id = i;
		philos[i]->thread = 0;
		philos[i]->data = data;
		philos[i]->meals_count = 0;
		philos[i]->time_last_meal = 0;
		p_assign_forks(philos[i]);
	}
	return (philos);
}

static int	p_init_global_mutexes(t_data *data)
{
	int	i;

	i = -1;
	data->locks_forks = malloc(sizeof(pthread_mutex_t) * data->philos_total);
	if (!data->locks_forks)
		return (p_util_error_print(ERR_MALLOC));
	else if (pthread_mutex_init(&data->lock_sim_stop, NULL) != SUCCESS)
		return (p_util_error_print(ERR_MUTEX));
	else if (pthread_mutex_init(&data->lock_log, NULL) != SUCCESS)
		return (p_util_error_print(ERR_MUTEX));
	while (++i < (int)data->philos_total)
		if (pthread_mutex_init(&(data->locks_forks[i]), NULL) != SUCCESS)
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
	p_init_global_mutexes(data);
	return (data);
}
