/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_other_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 11:38:44 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/09 19:31:04 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

size_t	p_util_get_time(void)
{
	static struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	p_util_usleep(size_t ms)
{
	size_t	end;

	end = p_util_get_time() + ms;
	while (p_util_get_time() < end)
		usleep(ms / 1000);
}

void	p_util_log(size_t timestamp, t_philo *philo, int action)
{
	pthread_mutex_lock(&philo->base_data->mutex_log);
	if (action == TAKING_FORK)
		printf("%zu %zu has taken a fork\n", timestamp, philo->position);
	else if (action == EATING)
		printf("%zu %zu is eating\n", timestamp, philo->position);
	else if (action == SLEEPING)
		printf("%zu %zu is sleeping\n", timestamp, philo->position);
	else if (action == THINKING)
		printf("%zu %zu is thinking\n", timestamp, philo->position);
	else if (action == DIED)	
		printf("%zu %zu died\n", timestamp, philo->position);
	else if (action == OVER)
		printf("%zu %zu has finished his meals\n", timestamp, philo->position);
	pthread_mutex_unlock(&philo->base_data->mutex_log);
}