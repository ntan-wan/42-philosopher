/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_log_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 10:38:42 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/21 10:39:55 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void    p_log_status_print(t_philo *philo, t_status status, char *color, char *stat)
{
	if (status == GET_FORK_1)
		printf("[%10ld]\t%s%3d\t%s\e[0m: fork [%d]\n",
			p_util_get_time_in_ms() - philo->data->time_start, color,
			philo->id + 1, stat, philo->fork[0]);
	else if (status == GET_FORK_2)
		printf("[%10ld]\t%s%3d\t%s\e[0m: fork [%d]\n",
			p_util_get_time_in_ms() - philo->data->time_start, color,
			philo->id + 1, stat, philo->fork[1]);
	else
		printf("[%10ld]\t%s%3d\t%s\e[0m\n",
			p_util_get_time_in_ms() - philo->data->time_start,
			color, philo->id + 1, stat);
}

void    p_log_status(t_philo * philo, t_status status)
{
	pthread_mutex_lock(&philo->data->lock_log);
	if (p_monitor_sim_has_stopped(philo->data))
	{
		pthread_mutex_unlock(&philo->data->lock_log);
		return ;
	}
	if (status == DIED)
		p_log_status_print(philo, status, RED, "died");
	else if (status == EATING)
		p_log_status_print(philo, status, GREEN, "is eating");
	else if (status == THINKING)
		p_log_status_print(philo, status, YELLOW, "is thinking");
	else if (status == SLEEPING)
		p_log_status_print(philo, status, COLOR_OFF, "is sleeping");
	else if (status == GET_FORK_1 || status == GET_FORK_2)
		p_log_status_print(philo, status, PURPLE, "has taken a fork");
	pthread_mutex_unlock(&philo->data->lock_log);
}