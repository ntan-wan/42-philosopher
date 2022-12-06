/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_philo_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 11:23:55 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/12/06 19:42:08 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	p_philo_get_forks(t_philo *philo)
{
	sem_wait(philo->data->sem_forks);
	sem_wait(philo->data->sem_forks);
	p_log_status(philo, GET_FORK_1);
	p_log_status(philo, GET_FORK_2);
}

void	p_philo_release_forks(t_philo *philo)
{
	sem_post(philo->data->sem_forks);
	sem_post(philo->data->sem_forks);
}

void	p_philo_eats(t_philo *philo)
{
	p_log_status(philo, EATING);
	sem_wait(philo->sem_meal);
	philo->time_last_meal = p_util_get_time_in_ms();
	sem_post(philo->sem_meal);
	p_util_usleep_sim_check(philo->data, philo->data->time_to_eat);
	sem_wait(philo->sem_meal);
	philo->meals_count++;
	sem_post(philo->sem_meal);
}

void	p_philo_sleeps(t_philo *philo)
{
	p_log_status(philo, SLEEPING);
	p_util_usleep_sim_check(philo->data, philo->data->time_to_sleep);
}

void	p_philo_thinks(t_philo *philo)
{
	p_log_status(philo, THINKING);
}
