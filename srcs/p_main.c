/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 11:06:25 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/08 10:50:46 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

















/*size_t get_time(void)
{
	static struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}*/

size_t	p_get_milisecond(void)
{
	static struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/*int	is_dead(t_philosopher *philo)
{
	if (!philo->eating && get_time() > philo->threshold)
	{
		ft_msg(philo, philo->threshold, DIED);
		pthread_mutex_lock(&philo->instance->write_mutex);
		pthread_mutex_unlock(&philo->instance->die_mutex);
		return (1);
	}
	return (0);
}*/

int	p_is_dead(t_philo *philo)
{
	if (!philo->eating && p_get_milisecond() > philo->threshold)
	{
		p_print_msg(philo, philo->threshold, DIED);
		pthread_mutex_lock(&philo->main_data->write_mutex);
		pthread_mutex_unlock(&philo->main_data->die_mutex);
		return (1);
	}
	return (0);
}

void	p_print_msg(t_philo *philo, size_t timestamp, int action)
{
	pthread_mutex_lock(&philo->main_data->write_mutex);
	if (action == TAKING_FORK)
		printf("%zu %zu has taken a fork\n", timestamp, philo->position);
	else if (action == EATING)
		printf("%zu %zu is eating\n", timestamp, philo->position);
	else if (action == SLEEPING)
		printf("%zu %zu is sleeping\n", timestamp, philo->position);
	else if (action == THINKING)
		printf("%zu %zu is thinking\n", timestamp, philo->position);
	else if (action == DIED)	
		printf("%zu %zu died\n", timestamp, philo->position);
	else if (action == OVER)
		printf("%zu %zu has finished his meals\n", timestamp, philo->position);
	pthread_mutex_unlock(&philo->main_data->write_mutex);
}

void	p_usleep(size_t ms)
{
	size_t	end;

	end = p_get_milisecond() + ms;
	while (p_get_milisecond() < end)
		usleep(ms / 1000);
}

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
	p_philo_routine_start(&data);
	pthread_mutex_lock(&data.die_mutex);
	pthread_mutex_unlock(&data.die_mutex);
	p_data_clear_all(&data);
	return (0);
}
