/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_init_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:17:18 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/12/04 23:11:08 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	p_init_data_parsing(t_data *data, char **av)
{
	if (av[5])
		data->meals_minimum = p_util_atoi(av[5]);
	else
		data->meals_minimum = 0;
	data->philos_total = p_util_atoi(av[1]);
	data->time_to_die = p_util_atoi(av[2]);
	data->time_to_eat = p_util_atoi(av[3]);
	data->time_to_sleep = p_util_atoi(av[4]);
	data->pids = malloc(sizeof(pid_t) * data->philos_total);
	data->sim_stop = false;
}

/* 
	combine "sem_local_meal" with philo->id to create unique semaphore name.
 */
static void	p_set_sim_meal(t_philo *philo)
{
	char	*temp_num;
	char	*sem_name;
	int		total_str_len;
	
	total_str_len = p_util_strlen(SEM_NAME_MEAL) + p_util_digit_count(philo->id + 1);
	sem_name = malloc(sizeof(char) * total_str_len + 1);
	sem_name[total_str_len] = '\0';
	temp_num = p_util_utoa(philo->id + 1);
	p_util_strcat(sem_name, SEM_NAME_MEAL);
	p_util_strcat(sem_name, temp_num);
	philo->sem_meal = sem_open(sem_name, O_CREAT, 0644, 1);
	free(sem_name);
	free(temp_num);
}

t_philo	**p_init_philos(t_data *data)
{
	int		i;
	t_philo	**philos;

	i = -1;
	philos = (t_philo **)malloc(sizeof(t_philo *) * data->philos_total);
	if (!philos)
		return (NULL);
	while (++i < (int)data->philos_total)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (NULL);
		philos[i]->id = i;
		philos[i]->data = data;
		philos[i]->meals_count = 0;
		philos[i]->time_last_meal = 0;
		philos[i]->death_check = 0;
		p_set_sim_meal(philos[i]);
	}
	return (philos);
}

static int	p_init_global_semaphores(t_data *data)
{
	data->sem_forks = sem_open(SEM_NAME_FORKS, O_CREAT , 0644, data->philos_total);
	sem_unlink(SEM_NAME_FORKS);
	data->sem_log = sem_open(SEM_NAME_LOG, O_CREAT, 0644, 1);
	sem_unlink(SEM_NAME_LOG);
	data->sem_sim_stop = sem_open(SEM_NAME_STOP, O_CREAT, 0644, 1);
	sem_unlink(SEM_NAME_STOP);
	return (SUCCESS);
}

t_data	*p_init_data(char **av)
{
	t_data	*data;

	data = malloc(sizeof(t_data) * 1);
	if (!data)
		return (NULL);
	p_init_data_parsing(data, av);
	p_init_global_semaphores(data);
	return (data);
}
