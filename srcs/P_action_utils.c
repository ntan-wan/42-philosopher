/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   P_action_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 20:36:29 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/13 22:27:10 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void    p_action_take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutex_forks[philo->fork_left]);
	p_util_log(p_util_get_time(philo->data), philo, TAKING_FORK);
	pthread_mutex_lock(&philo->data->mutex_forks[philo->fork_right]);
	p_util_log(p_util_get_time(philo->data), philo, TAKING_FORK);
}

void	p_action_release_forks(t_philo *philo)
{
	// p_util_log(p_util_get_time(philo->data), philo, RELEASE_FORK);
	pthread_mutex_unlock(&philo->data->mutex_forks[philo->fork_left]);
	// p_util_log(p_util_get_time(philo->data), philo, RELEASE_FORK);
	pthread_mutex_unlock(&philo->data->mutex_forks[philo->fork_right]);
}

void    p_action_eat(t_philo *philo)
{
	philo->time_last_meal = p_util_get_time(philo->data);
	philo->time_limit_to_eat = philo->time_last_meal + philo->data->time_to_die;
	p_util_log(p_util_get_time(philo->data), philo, EATING);
	usleep(philo->data->time_to_eat * 1000);
	p_action_release_forks(philo);
}

void    p_action_sleep(t_philo *philo)
{
	p_util_log(p_util_get_time(philo->data), philo, SLEEP);
	usleep(philo->data->time_to_sleep * 1000);
}

void    p_action_thinking(t_philo *philo)
{
	p_util_log(p_util_get_time(philo->data), philo, THINK);
	// p_util_log(0, philo, THINK);
}