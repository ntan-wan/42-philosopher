/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_philo_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 07:08:27 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/08 10:31:08 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	p_philo_take_forks(t_philo *philo)
{
	int		i;
	size_t	fork;
	
	i = 0;
	while (i < 2)
	{
		if (p_is_dead(philo))
			return (1);
		if (i == 0)
			fork = philo->left_fork;
		else
			fork = philo->right_fork;
		pthread_mutex_lock(&philo->main_data->forks_mutex[fork]);
		if (!philo->main_data->forks[fork])
		{
			p_print_msg(philo, p_get_milisecond(), TAKING_FORK);
			philo->main_data->forks[fork] = 1;
			i++;
		}
		pthread_mutex_unlock(&philo->main_data->forks_mutex[fork]);
	}
	return (0);
}

int	p_philo_eat(t_philo *philo)
{
	philo->eating = 1;
	philo->last_meal = p_get_milisecond();
	p_print_msg(philo, philo->last_meal, EATING);
	p_usleep(philo->main_data->time_to_eat);
	philo->threshold = philo->last_meal + philo->main_data->time_to_die;
	philo->eating = 0;
	philo->meals += 1;
	if (philo->main_data->meals > 0 && philo->meals >= philo->main_data->meals)
	{
		p_print_msg(philo, philo->last_meal, OVER);
		pthread_mutex_lock(&philo->main_data->forks_mutex[philo->left_fork]);
		philo->main_data->forks[philo->left_fork] = 0;
		pthread_mutex_unlock(&philo->main_data->forks_mutex[philo->left_fork]);
		pthread_mutex_lock(&philo->main_data->forks_mutex[philo->right_fork]);
		philo->main_data->forks[philo->right_fork] = 0;
		pthread_mutex_unlock(&philo->main_data->forks_mutex[philo->right_fork]);
		philo->main_data->finished++;
		if (philo->main_data->finished >= philo->main_data->philo_total)
		{
			pthread_mutex_lock(&philo->main_data->write_mutex);
			pthread_mutex_unlock(&philo->main_data->die_mutex);
		}
		return (1);
	}
	return (0);
}

int	p_philo_release_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->main_data->forks_mutex[philo->left_fork]);
	philo->main_data->forks[philo->left_fork] = 0;
	pthread_mutex_unlock(&philo->main_data->forks_mutex[philo->left_fork]);
	pthread_mutex_lock(&philo->main_data->forks_mutex[philo->right_fork]);
	philo->main_data->forks[philo->right_fork] = 0;
	pthread_mutex_unlock(&philo->main_data->forks_mutex[philo->right_fork]);
	p_print_msg(philo, philo->last_meal + philo->main_data->time_to_eat, SLEEPING);
	p_usleep(philo->main_data->time_to_sleep);
	if (p_is_dead(philo))
		return (1);
	p_print_msg(philo, philo->last_meal + philo->main_data->time_to_eat + philo->main_data->time_to_sleep, THINKING);
	return (0);
}

void	*p_philo_routine_do(void *philosopher)
{
	t_philo	*philo;

	philo = (t_philo *)philosopher;
	philo->last_meal = p_get_milisecond();
	philo->threshold = philo->last_meal + philo->main_data->time_to_die;
	while (1)
	{
		if (p_philo_take_forks(philo) || p_philo_eat(philo) || p_philo_release_forks(philo))
			break ;
	}
	return (NULL);
}

void	p_philo_routine_start(t_data *data)
{
	size_t		i;
	pthread_t	tid;

	i = 0;
	pthread_mutex_lock(&data->die_mutex);
	while (i < data->philo_total)
	{
		pthread_create(&tid, NULL, p_philo_routine_do, (void *)&data->philos[i]);
		pthread_detach(tid);
		if (data->philo_total % 2)
			p_usleep(500);
		i++;
	}
}