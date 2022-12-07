/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_other_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:38:43 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/12/07 17:33:31 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"
#define ZERO_POINT_ONE_MS 100

int	p_util_atoi(char *str)
{
	unsigned long long int	num;

	num = 0;
	while (*str && *str >= '0' && *str <= '9')
		num = (num * 10) + (*str++ - '0');
	if (num > INT_MAX)
		return (-1);
	else
		return ((int)num);
}

time_t	p_util_get_time_in_ms(void)
{
	struct timeval		tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	p_util_delay(time_t time_start)
{
	while (p_util_get_time_in_ms() < time_start)
		continue ;
}

void	p_util_usleep_sim_check(t_data *data, time_t time_to_sleep)
{
	time_t	time_wake_up;

	time_wake_up = p_util_get_time_in_ms() + time_to_sleep;
	while (p_util_get_time_in_ms() < time_wake_up)
	{
		if (p_monitor_sim_has_stopped(data))
			break ;
		usleep(ZERO_POINT_ONE_MS);
	}
}

void	p_util_kill_all_philos(t_data *data)
{
	int	i;

	i = -1;
	while (data->pids[++i])
		kill(data->pids[i], SIGINT);
}
