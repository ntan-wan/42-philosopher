/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_other_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 07:02:08 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/12/01 15:24:25 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

/*void	p_util_destroy_mutexes(t_data *data, t_philo **philos)
{
	int	i;

	i = -1;
	while (++i < (int)data->philos_total)
	{
		pthread_mutex_destroy(&data->locks_forks[i]);
		pthread_mutex_destroy(&philos[i]->lock_meal_time);
	}
	pthread_mutex_destroy(&data->lock_log);
	pthread_mutex_destroy(&data->lock_sim_stop);
}*/

/*void	p_util_free_data(t_data **all_data)
{
	t_data	*data;

	data = *all_data;
	if (!data)
		return ;
	if (data->locks_forks)
		free(data->locks_forks);
	free(data);
	*all_data = NULL;
}*/

void	p_util_free_philos(t_philo **philos)
{
	int		i;
	t_data	*data;

	i = -1;
	data = philos[0]->data;
	while (philos && ++i < (int)data->philos_total)
		if (philos[i])
			free(philos[i]);
	free(philos);
}

void	p_util_unlink_global_sems(void)
{
	sem_unlink(SEM_NAME_SIM);
	sem_unlink(SEM_NAME_LOG);
	sem_unlink(SEM_NAME_FORKS);
}

int	p_util_digit_count(unsigned int num)
{
	int	digit_count;

	digit_count = 0;
	if (num == 0)	
		digit_count = 1;
	while (num)
	{
		num /= 10;
		digit_count++;
	}
	return (digit_count);
}

int	p_util_strlen(char *str)
{
	int	str_len;
	
	str_len = 0;
	while (*str)
	{
		str_len++;
		str++;
	}
	return (str_len);
}

void	p_util_strcat(char *dst, char *src)
{
	if (!dst || !src)
		return ;
	while (*dst)
		dst++;
	while (*src)
		*dst++ = *src++;
}

char	*p_util_utoa(unsigned int num)
{
	char	*str_num;
	int		str_len;

	str_len = p_util_digit_count(num);
	str_num = (char *)malloc(sizeof(char) * (str_len + 1));
	if (!str_num)
		return (NULL);
	str_num[0] = '0';
	str_num[str_len] = '\0';
	while (num % 10)
	{
		str_num[--str_len] = num % 10 + '0';
		num /= 10;
	}
	return (str_num);
}