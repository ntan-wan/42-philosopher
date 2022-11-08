/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_philo_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 07:08:27 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/08 11:47:22 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	p_philo_is_dead(t_philo *philo)
{
	if (!philo->eating && p_util_get_milisecond() > philo->threshold)
	{
		p_util_log(philo, philo->threshold, DIED);
		pthread_mutex_lock(&philo->base_data->write_mutex);
		pthread_mutex_unlock(&philo->base_data->die_mutex);
		return (1);
	}
	return (0);
}

int	p_philo_take_forks(t_philo *philo)
{
	int		i;
	size_t	fork;
	
	i = 0;
	while (i < 2)
	{
		if (p_philo_is_dead(philo))
			return (1);
		if (i == 0)
			fork = philo->left_fork;
		else
			fork = philo->right_fork;
		pthread_mutex_lock(&philo->base_data->forks_mutex[fork]);
		if (!philo->base_data->forks[fork])
		{
			p_util_log(philo, p_util_get_milisecond(), TAKING_FORK);
			philo->base_data->forks[fork] = 1;
			i++;
		}
		pthread_mutex_unlock(&philo->base_data->forks_mutex[fork]);
	}
	return (0);
}

int	p_philo_eat(t_philo *philo)
{
	philo->eating = 1;
	philo->last_meal = p_util_get_milisecond();
	p_util_log(philo, philo->last_meal, EATING);
	p_util_usleep(philo->base_data->time_to_eat);
	philo->threshold = philo->last_meal + philo->base_data->time_to_die;
	philo->eating = 0;
	philo->meals += 1;
	if (philo->base_data->meals > 0 && philo->meals >= philo->base_data->meals)
	{
		p_util_log(philo, philo->last_meal, OVER);
		pthread_mutex_lock(&philo->base_data->forks_mutex[philo->left_fork]);
		philo->base_data->forks[philo->left_fork] = 0;
		pthread_mutex_unlock(&philo->base_data->forks_mutex[philo->left_fork]);
		pthread_mutex_lock(&philo->base_data->forks_mutex[philo->right_fork]);
		philo->base_data->forks[philo->right_fork] = 0;
		pthread_mutex_unlock(&philo->base_data->forks_mutex[philo->right_fork]);
		philo->base_data->finished++;
		if (philo->base_data->finished >= philo->base_data->philo_total)
		{
			pthread_mutex_lock(&philo->base_data->write_mutex);
			pthread_mutex_unlock(&philo->base_data->die_mutex);
		}
		return (1);
	}
	return (0);
}

int	p_philo_release_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->base_data->forks_mutex[philo->left_fork]);
	philo->base_data->forks[philo->left_fork] = 0;
	pthread_mutex_unlock(&philo->base_data->forks_mutex[philo->left_fork]);
	pthread_mutex_lock(&philo->base_data->forks_mutex[philo->right_fork]);
	philo->base_data->forks[philo->right_fork] = 0;
	pthread_mutex_unlock(&philo->base_data->forks_mutex[philo->right_fork]);
	p_util_log(philo, philo->last_meal + philo->base_data->time_to_eat, SLEEPING);
	p_util_usleep(philo->base_data->time_to_sleep);
	if (p_philo_is_dead(philo))
		return (1);
	p_util_log(philo, philo->last_meal + philo->base_data->time_to_eat + philo->base_data->time_to_sleep, THINKING);
	return (0);
}
