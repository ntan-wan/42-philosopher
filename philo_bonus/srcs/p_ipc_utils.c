/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_ipc_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 07:02:10 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/12/04 00:28:04 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

static bool philo_open_global_sems(t_philo *philo)
{
	t_data  *data;

	// printf("%d\n", philo->data->philos_total);
	philo->sem_forks = sem_open(SEM_NAME_FORKS, O_CREAT, S_IRUSR | S_IWUSR, philo->data->philos_total);
	philo->sem_log = sem_open(SEM_NAME_LOG, O_CREAT, S_IRUSR | S_IWUSR, 1);
	philo->sem_philo_full = sem_open(SEM_NAME_FULL, O_CREAT, S_IRUSR | S_IWUSR, philo->data->philos_total);
	philo->sem_philo_dead = sem_open(SEM_NAME_DEAD, O_CREAT, S_IRUSR | S_IWUSR, philo->data->philos_total);
	if (philo->sem_forks == SEM_FAILED || philo->sem_log == SEM_FAILED)
		return (false);
	else
		return (true);
}

static bool philo_open_local_sems(t_philo *philo)
{
	philo->sem_meal = sem_open(philo->sem_meal_name, O_CREAT, S_IRUSR | S_IWUSR, 1);
	if (philo->sem_meal == SEM_FAILED)
		return (false);
	sem_unlink(philo->sem_meal_name);
		return (true);
}

bool    p_death_check_ended(t_data *data, t_philo *philo)
{
	/*time_t	time_current;
	time_t	time_passed;

	sem_wait(philo->sem_meal);
	time_current = p_util_get_time_in_ms();
	time_passed = time_current - philo->time_last_meal;
	if (time_passed > philo->data->time_to_die)
	{
		p_log_status(philo, DIED);
		p_log_death_report(time_current, philo);
		p_monitor_set_sim_stop(philo->data, true);
		sem_post(philo->sem_meal);
		// pthread_mutex_unlock(&philo->lock_meal_time);
		return (true);
	}
	sem_post(philo->sem_meal);
	return (false);*/
	// printf("testing\n");
	sem_wait(philo->sem_meal);
	if (p_util_get_time_in_ms() - philo->time_last_meal > data->time_to_die)
	{
		p_log_status(philo, DIED);
		sem_post(data->this_philo->sem_philo_dead);
		sem_post(philo->sem_meal);
		return (true);
	}
	if (data->meals_minimum && philo->ate_enough == false && philo->meals_count >= data->meals_minimum)
	{
		sem_post(philo->sem_philo_full);
		philo->ate_enough = true;
	}
	sem_post(philo->sem_meal);
	return (false);
}

void    *p_death_check(void *all_data)
{
	t_data  *data;

	data = (t_data *)all_data;
	// if (data->meals_minimum == 0)
		// return (NULL);
	// sem_post(data->this_philo->sem_philo_dead);
	// sem_post(data->this_philo->sem_philo_full);
	// printf("%d\n", data->this_philo->sem_philo_dead);
	sem_wait(data->this_philo->sem_philo_dead);
	sem_wait(data->this_philo->sem_philo_full);
	p_util_delay(data->time_start);
	while (!p_death_check_ended(data, data->this_philo))
	{
		usleep(1000);
		continue ;
	}
	return (NULL);
}

/* 
	init interprocess.
 */
void    p_ipc_philo_init(t_data *data, t_philo *philo)
{
	sem_unlink(philo->sem_meal_name);
	philo_open_global_sems(philo);
	philo_open_local_sems(philo);
	pthread_create(&philo->death_check, NULL, p_death_check, data);
}