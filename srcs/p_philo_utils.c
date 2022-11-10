/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_philo_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 07:08:27 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/09 20:16:24 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int	p_philo_is_dead(t_philo *philo)
{
	if (!philo->eating && p_util_get_time() > philo->time_limit_to_eat)
	{
		pthread_mutex_lock(philo->eat);
		p_util_log(philo->time_limit_to_eat, philo, DIED);
		// pthread_mutex_lock(&philo->base_data->mutex_log);
		pthread_mutex_unlock(&philo->base_data->mutex_routine_end);
		return (DIED);
	}
	return (0);
}

/*int	p_philo_take_forks(t_philo *philo)
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
		pthread_mutex_lock(&philo->base_data->mutex_forks[fork]);
		if (!philo->base_data->forks[fork])
		{
			p_util_log(p_util_get_time(), philo, TAKING_FORK);
			philo->base_data->forks[fork] = 1;
			i++;
		}
		pthread_mutex_unlock(&philo->base_data->mutex_forks[fork]);
	}
	return (0);
}*/


/*int	p_philo_eat(t_philo *philo)
{
	philo->eating = 1;
	philo->time_last_meal = p_util_get_time();
	p_util_log(philo->time_last_meal, philo, EATING);
	p_util_usleep(philo->base_data->time_to_eat);
	philo->time_limit_to_eat = philo->time_last_meal + philo->base_data->time_to_die;
	philo->eating = 0;
	philo->meals += 1;
	if (philo->base_data->max_meals > 0 && philo->meals >= philo->base_data->max_meals)
	{
		p_util_log(philo->time_last_meal, philo, OVER);
		pthread_mutex_lock(&philo->base_data->mutex_forks[philo->left_fork]);
		philo->base_data->forks[philo->left_fork] = 0;
		pthread_mutex_unlock(&philo->base_data->mutex_forks[philo->left_fork]);
		pthread_mutex_lock(&philo->base_data->mutex_forks[philo->right_fork]);
		philo->base_data->forks[philo->right_fork] = 0;
		pthread_mutex_unlock(&philo->base_data->mutex_forks[philo->right_fork]);
		philo->base_data->finished++;
		if (philo->base_data->finished >= philo->base_data->philo_total)
		{
			pthread_mutex_lock(&philo->base_data->mutex_log);
			pthread_mutex_unlock(&philo->base_data->mutex_routine_end);
		}
		return (1);
	}
	return (0);
}*/

void	p_philo_forks_take(t_philo *philo)
{
	pthread_mutex_lock(&philo->base_data->mutex_forks[philo->left_fork]);
	p_util_log(p_util_get_time(), philo, TAKING_FORK);
	pthread_mutex_lock(&philo->base_data->mutex_forks[philo->right_fork]);
	p_util_log(p_util_get_time(), philo, TAKING_FORK);
}

void	p_philo_forks_release(t_philo *philo)
{
	pthread_mutex_unlock(&philo->base_data->mutex_forks[philo->left_fork]);
	pthread_mutex_unlock(&philo->base_data->mutex_forks[philo->right_fork]);
}


void	p_philo_eat(t_philo *philo)
{    
	pthread_mutex_lock(philo->eat);
	if (philo->base_data->meals_minimum)
		philo->base_data->meals_total_all_eaten++;
	philo->time_last_meal = p_util_get_time();
	philo->time_limit_to_eat = philo->time_last_meal + philo->base_data->time_to_die;
	p_util_log(philo->time_last_meal, philo, EATING);
	usleep(philo->base_data->time_to_eat * 1000);
	p_philo_forks_release(philo);
	pthread_mutex_unlock(philo->eat);
	
}

void	p_philo_sleep(t_philo *philo)
{
	//
	usleep(10);
	p_util_log(p_util_get_time(), philo, SLEEPING);
	usleep(philo->base_data->time_to_sleep * 1000);
}


/*int	p_philo_release_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->base_data->mutex_forks[philo->left_fork]);
	philo->base_data->forks[philo->left_fork] = 0;
	pthread_mutex_unlock(&philo->base_data->mutex_forks[philo->left_fork]);
	pthread_mutex_lock(&philo->base_data->mutex_forks[philo->right_fork]);
	philo->base_data->forks[philo->right_fork] = 0;
	pthread_mutex_unlock(&philo->base_data->mutex_forks[philo->right_fork]);
	p_util_log(philo->time_last_meal + philo->base_data->time_to_eat, philo, SLEEPING);
	p_util_usleep(philo->base_data->time_to_sleep);
	if (p_philo_is_dead(philo))
		return (1);
	p_util_log(philo->time_last_meal + philo->base_data->time_to_eat + philo->base_data->time_to_sleep, philo, THINKING);
	return (0);
}*/
