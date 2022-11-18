/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_routine_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 09:34:05 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/18 15:47:14 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void    *p_routine_philo_one(t_philo *philo)
{
    pthread_mutex_lock(&philo->data->locks_forks[philo->fork[0]]);
    p_log_status(philo, GOT_FORK_1);
    p_philo_sleep(philo->data, philo->data->time_to_die);
    p_log_status(philo, DIED);
    pthread_mutex_unlock(&philo->data->locks_forks[philo->fork[0]]);
    return (NULL);
}

void    *p_routine_philo(void *philosopher)
{
    t_philo *philo;

    philo = (t_philo *)data;
    pthread_mutex_lock(&philo->lock_meal_time);
    philo->time_last_meal = philo->data->time_start;
    pthread_mutex_unlock(&philo->lock_meal_time);
    p_util_delay(philo->data->time_start);
    if (philo->data->philos_total == 1)
        return (p_routine_philo_one(philo));
}

int	p_routine_start(t_data *data)
{
	unsigned int	i;

	data->time_start = p_util_get_time_in_ms() + (data->philos_total * 2 * 10);
	i = 0;
	while (i < data->philos_total)
	{
		if (pthread_create(&data->philos[i]->thread, NULL, p_routine_philo, data->philos[i]) != 0)
			return (p_util_error_print(ERR_THREAD));
		i++;
	}
	return (SUCCESS);
}