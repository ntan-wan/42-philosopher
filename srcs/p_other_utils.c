/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_other_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 11:38:08 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/13 09:15:20 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	p_util_log(size_t timestamp, t_philo *philo, int action)
{
	if (action == TAKING_FORK)
		printf("%ldms %ld has taken a fork\n", timestamp, philo->position_num);
	else if (action == RELEASE_FORK)
		printf("%ldms %ld has release a fork\n", timestamp, philo->position_num);
	else if (action == EATING)
		printf("%ldms %ld is eating\n", timestamp, philo->position_num);
	else if (action == SLEEP)
		printf("%ldms %ld is sleeping\n", timestamp, philo->position_num);
	else if (action == THINK)
		printf("%ldms %ld is thinking\n", timestamp, philo->position_num);
	else if (action == DIED)
		printf("%ldms %ld died\n", timestamp, philo->position_num);
}

size_t	p_util_get_millisecond(void)
{
	static struct timeval	time_value;
	
	gettimeofday(&time_value, NULL);
	return ((time_value.tv_sec * 1000) + (time_value.tv_usec / 1000));
}

size_t	p_util_get_time(t_data *data)
{
	return (p_util_get_millisecond() - data->time_routine_start);
}

int	p_util_error_print(char *msg)
{
	printf("%s\n", msg);
	return (ERROR);
}

size_t  p_util_a_to_unsigned_int(char *str)
{
	size_t  num;
	
	if (!str)
		return (ERROR);
	num = 0;
	while (*str == '+')
		str++;
	while (*str)
	{
	   if (*str >= '0' && *str <= '9')
			num = num * 10 + ( *str - '0');
		else
			return (ERROR);
		str++;
	}
	return (num);
}