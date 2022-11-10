/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_routine.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 11:34:56 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/09 20:17:05 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/*void	*p_routine_loop(void *philosopher)
{
	t_philo	*philo;

	philo = (t_philo *)philosopher;
	philo->time_last_meal = p_util_get_time();
	philo->time_limit_to_eat = philo->time_last_meal + philo->base_data->time_to_die;
	while (1)
	{
		if (p_philo_take_forks(philo) || p_philo_eat(philo) || p_philo_release_forks(philo))
			break ;
	}
	return (NULL);
}*/


static void	*p_death_check(void	*philosopher)
{
	t_philo	*philo;

	philo = (t_philo *)philosopher;
	while (1)
	{
		// if (!philo->eating && p_util_get_time() > philo->time_limit_to_eat)
		if (p_util_get_time() > philo->time_limit_to_eat)
		{
			philo->base_data->philo_died = 1;
			pthread_mutex_lock(philo->eat);
			p_util_log(p_util_get_time(), philo, DIED);
			pthread_mutex_unlock(&philo->base_data->mutex_routine_end);
			break;
		}
	}
	return (NULL);
}

int	meals_minimum_check(t_philo *philo)
{
	if (philo->base_data->meals_minimum
		&& philo->base_data->meals_total_all_eaten >= philo->base_data->meals_total_all)
	{
		pthread_mutex_lock(philo->eat);
		p_util_log();
		pthread_mutex_unlock(&philo->base_data->mutex_routine_end);
		return (0);
	}
	return (1);
}

void	*p_routine_loop(void *philosopher)
{
	t_philo 	*philo;
	pthread_t	death_check;

	philo = (t_philo *)philosopher;
	philo->time_last_meal = p_util_get_time();
	philo->time_limit_to_eat = philo->time_last_meal + philo->base_data->time_to_die;
	pthread_create(&death_check, NULL, p_death_check, philo);
	pthread_detach(death_check);
	while (1)
	{
		if (philo->base_data->philo_died)
			break ;
		p_philo_forks_take(philo);
		p_philo_eat(philo);
		if (meals_minimum_check() == 0)
			break ;
		p_philo_sleep(philo);
		p_util_log(p_util_get_time(), philo, THINKING);
	}
	return (NULL);
}

void	p_routine_start(t_data *data)
{
	size_t		i;
	pthread_t	tid;

	i = 0;
	while (i < data->philo_total)
	{
		pthread_create(&tid, NULL, p_routine_loop, (void *)&data->philos[i]);
		pthread_detach(tid);
		usleep(100);
		// if (data->philo_total % 2)
			// p_util_usleep(500);
		i++;
	}
}