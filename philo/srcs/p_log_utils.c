/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_log_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 10:38:42 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/23 19:30:24 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	p_print(t_philo *philo, t_status status, char *color, char *stat)
{
	if (status == GET_FORK_1)
		printf("[%10ld]\t%s%3d\t%s\e[0m: fork [%d]\n",
			p_util_get_time_in_ms() - philo->data->time_start, color,
			philo->id + 1, stat, philo->fork[0] + 1);
	else if (status == GET_FORK_2)
		printf("[%10ld]\t%s%3d\t%s\e[0m: fork [%d]\n",
			p_util_get_time_in_ms() - philo->data->time_start, color,
			philo->id + 1, stat, philo->fork[1] + 1);
	else
		printf("[%10ld]\t%s%3d\t%s\e[0m\n",
			p_util_get_time_in_ms() - philo->data->time_start,
			color, philo->id + 1, stat);
}

void	p_log_status(t_philo *philo, t_status status)
{
	pthread_mutex_lock(&philo->data->lock_log);
	if (p_monitor_sim_has_stopped(philo->data))
	{
		pthread_mutex_unlock(&philo->data->lock_log);
		return ;
	}
	if (status == DIED)
		p_print(philo, status, RED, "died");
	else if (status == THINKING)
		p_print(philo, status, YELLOW, "is thinking");
	else if (status == GET_FORK_1 || status == GET_FORK_2)
		p_print(philo, status, PURPLE, "has taken a fork");
	else if (status == EATING)
		p_print(philo, status, GREEN, "is eating");
	else if (status == SLEEPING)
		p_print(philo, status, COLOR_OFF, "is sleeping");
	pthread_mutex_unlock(&philo->data->lock_log);
}

void	p_log_death_report(time_t time_current, t_philo *philo)
{
	time_t	time_passed;

	time_passed = time_current - philo->time_last_meal;
	printf("\n%sDeath report%s\n", BOLD_AND_RED_BG, COLOR_OFF);
	printf("%sPhilo %d\n", BOLD, philo->id + 1);
	printf("Time_to_die = %ld\n", philo->data->time_to_die);
	printf("Time_current = %ld\n", time_current);
	printf("Time_last_meal = %ld\n", philo->time_last_meal);
	printf("Time_passed = %ld%s\n", time_passed, COLOR_OFF);
}

void	p_log_meals_report(t_philo **philos)
{
	int		i;
	t_data	*data;
	
	i = -1;
	data = philos[0]->data;
	while (++i < (int)data->philos_total)
		printf("philo %d has ate %d meals\n", philos[i]->id + 1, philos[i]->meals_count);
}