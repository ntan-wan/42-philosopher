/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_philo_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 11:23:55 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/21 18:28:23 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	p_philo_get_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->locks_forks[philo->fork[0]]);
	pthread_mutex_lock(&philo->data->locks_forks[philo->fork[1]]);
	p_log_status(philo, GET_FORK_1);
	p_log_status(philo, GET_FORK_2);
}

void	p_philo_release_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->data->locks_forks[philo->fork[1]]);
	pthread_mutex_unlock(&philo->data->locks_forks[philo->fork[0]]);
}

void	p_philo_eats(t_philo *philo)
{
	p_log_status(philo, EATING);
	pthread_mutex_lock(&philo->lock_meal_time);
	philo->time_last_meal = p_util_get_time_in_ms();
	pthread_mutex_unlock(&philo->lock_meal_time);
	p_util_usleep(philo->data, philo->data->time_to_eat);
	pthread_mutex_lock(&philo->lock_meal_time);
	philo->meals_count++;
	pthread_mutex_unlock(&philo->lock_meal_time);
}

void	p_philo_sleeps(t_philo *philo)
{
	p_log_status(philo, SLEEPING);
	p_util_usleep(philo->data, philo->data->time_to_sleep);
}

void	p_philo_thinks(t_philo *philo)
{
	p_log_status(philo, THINKING);
}
