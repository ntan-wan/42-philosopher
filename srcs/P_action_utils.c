/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   P_action_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 20:36:29 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/14 17:57:14 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void    p_action_take_forks(t_philo *philo)
{
	int	i;

	i = -1;
	pthread_mutex_lock(&philo->data->mutex_forks[philo->fork_left]);
	pthread_mutex_lock(&philo->data->mutex_forks[philo->fork_right]);
	while (++i < 2)
		p_util_log(p_util_get_time(philo->data), philo, TAKING_FORK);
}

void	p_action_release_forks(t_philo *philo)
{
	int	i;

	i = -1;
	while (++i < 2)
		p_util_log(p_util_get_time(philo->data), philo, RELEASE_FORK);
	pthread_mutex_unlock(&philo->data->mutex_forks[philo->fork_left]);
	pthread_mutex_unlock(&philo->data->mutex_forks[philo->fork_right]);
}

void    p_action_eat(t_philo *philo)
{
	philo->time_last_meal = p_util_get_time(philo->data);
	philo->time_limit_to_eat = philo->time_last_meal + philo->data->time_to_die;
	p_util_log(p_util_get_time(philo->data), philo, EATING);
	usleep(philo->data->time_to_eat * 1000);
	philo->meals_count++;
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
}