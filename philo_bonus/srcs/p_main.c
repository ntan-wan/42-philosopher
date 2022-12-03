/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:05:16 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/12/04 00:24:51 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"
int	kill_all_philos(t_data *table, int exit_code);

/*static int	p_monitor_start(pthread_t *thread_monitor, t_philo **philos)
{
	t_data	*data;
	
	data = philos[0]->data;
	if (data->philos_total > 1)
	{
		// printf("ihello\n");
		// return (SUCCESS);
		if (pthread_create(thread_monitor, NULL,
				p_monitor_philo, philos) != 0)
			return (p_util_error_print(ERR_THREAD));
		// pthread_detach(*thread_monitor);
	}
	return (SUCCESS);
}*/

void	*p_gluttony_reaper(void *all_data)
{
	t_data	*data;

	data = (t_data *)all_data;
	if (data->philos_total == 1)
		return (NULL);
	p_util_delay(data->time_start);
	while (data->philo_full_count < data->philos_total)
	{
		// printf("%d\n", data->philo_full_count);
		if (p_monitor_sim_has_stopped(data))
			return (NULL);
		sem_wait(data->sem_philo_full);
		if (!p_monitor_sim_has_stopped(data))
			data->philo_full_count += 1;
		else
			return (NULL);
	}
	sem_wait(data->sem_sim_stop);
	data->sim_stop = true;
	kill_all_philos(data, EXIT_SUCCESS);
	sem_post(data->sem_philo_dead);
	sem_post(data->sem_sim_stop);
	return (NULL);
}

void	*p_famine_reaper(void *all_data)
{
	t_data	*data;

	data = (t_data *)all_data;
	if (data->philos_total == 1)
		return (NULL);
	p_util_delay(data->time_start);
	if (p_monitor_sim_has_stopped(data))
		return (NULL);
	sem_wait(data->sem_philo_dead);
	if (p_monitor_sim_has_stopped(data))
		return (NULL);
	sem_wait(data->sem_sim_stop);
	data->sim_stop = true;
	kill_all_philos(data, EXIT_SUCCESS);
	sem_post(data->sem_philo_full);
	sem_post(data->sem_sim_stop);
	return (NULL);
}

void	p_start_monitor_threads(t_data *data)
{
	pthread_create(&data->gluttony_reaper, NULL, p_gluttony_reaper, data);
	pthread_create(&data->famine_reaper, NULL, p_famine_reaper, data);
	// return (true);
}

static int	p_sim_start(t_data *data, t_philo **philos)
{
	int		i;
	pid_t	pid;
	time_t	time_adjustment;

	i = -1;
	time_adjustment = (data->philos_total * 2) * 10;
	data->time_start = p_util_get_time_in_ms() + time_adjustment;
	while (++i < (int)data->philos_total)
	{
		pid = fork();
		if (pid == -1)
			return (p_util_error_print("fork error"));
		else if (pid > 0)
			data->pids[i] = pid;
		else if (pid == 0)
		{
			data->this_philo = data->philos[i];
			p_routine_philo(data);
		}
	}
	p_start_monitor_threads(data);
	// return (true);
	return (SUCCESS);
}

/*static void	p_sim_end(t_data **data, pthread_t thread_monitor, t_philo **philos)
{
	int	i;

	i = -1;
	while (++i < (int)(*data)->philos_total)
		pthread_join(philos[i]->thread, NULL);
	if ((*data)->philos_total > 1)
		pthread_join(thread_monitor, NULL);
	if ((*data)->meals_minimum)
		p_log_meals_report(philos);
	p_util_destroy_mutexes(*data, philos);
	p_util_free_philos(philos);
	p_util_free_data(data);
}*/

int	kill_all_philos(t_data *table, int exit_code)
{
	unsigned int	i;

	i = 0;
	while (i < table->philos_total)
	{
		kill(table->pids[i], SIGKILL);
		i++;
	}
	return (exit_code);
}

static int	get_child_philo(t_data *table, pid_t *pid)
{
	int	philo_exit_code;
	int	exit_code;

	if (*pid && waitpid(*pid, &philo_exit_code, WNOHANG) != 0)
	{
		if (WIFEXITED(philo_exit_code))
		{
			exit_code = WEXITSTATUS(philo_exit_code);
			if (exit_code == CHILD_EXIT_PHILO_DEAD)
				return (kill_all_philos(table, 1));
			if (exit_code == CHILD_EXIT_ERR_PTHREAD
				|| exit_code == CHILD_EXIT_ERR_SEM)
				return (kill_all_philos(table, -1));
			if (exit_code == CHILD_EXIT_PHILO_FULL)
			{
				table->philo_full_count += 1;
				return (1);
			}
		}
	}
	return (0);
}

void	p_sim_end(t_data **data, pthread_t thread_monitor)
{
	int	i;
	int	exit_code;

	p_util_delay((*data)->time_start);
	while (p_monitor_sim_has_stopped(*data) == false)
	{
		i = -1;
		while (++i < (int)(*data)->philos_total)
		{
			// printf("%d\n", i);
			exit_code = get_child_philo(*data, &(*data)->pids[i]);
			// printf("exit_code %d\n", exit_code);
			if (exit_code == 1 || exit_code == -1)
			{
				sem_wait((*data)->sem_sim_stop);
				(*data)->sim_stop = true;
				sem_post((*data)->sem_philo_full);
				sem_post((*data)->sem_philo_dead);
				sem_post((*data)->sem_sim_stop);
				// return ;
				// return (exit_code);
			}
		}
	}
	// return ;
	// return (0);
	// if ((*data)->philos_total > 1)
		// pthread_join(thread_monitor, NULL);
	// kill(-1, SIGKILL);
	// sem_wait(data->sem_sim_stop);
}

int	main(int ac, char **av)
{
	t_data		*data;
	t_philo		**philos;
	pthread_t	thread_monitor;

	data = NULL;
	if (is_invalid_input(ac, av))
		return (EXIT_FAILURE);
	data = p_init_data(av);
	// philos = p_init_philos(data);
	if (!data)
		return (EXIT_FAILURE);
	else if (p_sim_start(data, philos) == ERROR)
		return (EXIT_FAILURE);
	// else if (p_monitor_start(&thread_monitor, philos) == ERROR)
		// return (EXIT_FAILURE);
	p_sim_end(&data, thread_monitor);
	pthread_join(data->famine_reaper, NULL);
	pthread_join(data->gluttony_reaper, NULL);
	return (EXIT_SUCCESS);
}
