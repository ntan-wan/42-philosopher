/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_philo_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 11:23:55 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/12/04 00:20:36 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	p_philo_get_forks(t_philo *philo)
{
	// pthread_mutex_lock(&philo->data->locks_forks[philo->fork[0]]);
	// pthread_mutex_lock(&philo->data->locks_forks[philo->fork[1]]);
	// t_data	*data;
	
	// data = philo->data;
	// sem_wait(data->sem_forks);
	// sem_wait(data->sem_forks);
	// printf("%s\n", philo->sem_meal_name);
	// printf("%p\n", philo->sem_forks);
	sem_wait(philo->sem_forks);
	sem_wait(philo->sem_forks);
	sem_wait(philo->sem_meal);
	// printf("hello\n");
	p_log_status(philo, GET_FORK_1);
	p_log_status(philo, GET_FORK_2);
	sem_post(philo->sem_meal);
}

void	p_philo_release_forks(t_philo *philo)
{
	// pthread_mutex_unlock(&philo->data->locks_forks[philo->fork[1]]);
	// pthread_mutex_unlock(&philo->data->locks_forks[philo->fork[0]]);
	// t_data	*data;

	// data = philo->data;
	sem_post(philo->sem_forks);
	sem_post(philo->sem_forks);
}

void	p_philo_eats(t_philo *philo)
{
	p_log_status(philo, EATING);
	// pthread_mutex_lock(&philo->lock_meal_time);
	sem_wait(philo->sem_meal);
	philo->time_last_meal = p_util_get_time_in_ms();
	sem_post(philo->sem_meal);
	// pthread_mutex_unlock(&philo->lock_meal_time);
	p_util_usleep_sim_check(philo->data, philo->data->time_to_eat);
	// pthread_mutex_lock(&philo->lock_meal_time);
	// p_log_status(philo, SLEEPING);
	// sem_post(philo->sem_forks);
	// sem_post(philo->sem_forks);
	sem_wait(philo->sem_meal);
	philo->meals_count++;
	// printf("%d\n", philo->meals_count);
	sem_post(philo->sem_meal);
	// pthread_mutex_unlock(&philo->lock_meal_time);
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

/*void	p_philo_death_check(void *data)
{
	t_data	*data;	

	data = (t_data *)data;
}*/