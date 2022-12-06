/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:05:16 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/12/06 17:29:27 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/*static int	p_monitor_start(pthread_t *thread_monitor, t_philo **philos)
{
	if (philos[0]->data->philos_total > 1)
	{
		if (pthread_create(thread_monitor, NULL,
				p_monitor_philo, philos) != 0)
			return (p_util_error_print(ERR_THREAD));
	}
	return (SUCCESS);
}*/

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
		if (pid > 0)
			data->pids[i] = pid;
		else if (pid == 0)
			p_routine_philo(philos[i]);
	}
	return (SUCCESS);
}

static void	p_sim_end(t_data **data, pthread_t thread_monitor, t_philo **philos)
{
	// while (waitpid(-1, NULL, 0) != -1)
		// ;
	// p_kill_child_philos(*data);
	int philo_status;

	philo_status = 0;
	while (waitpid(-1, &philo_status, 0) != -1)
	{
		// if (WEXITSTATUS(philo_status) == PHILO_IS_DEAD)
		if (WEXITSTATUS(philo_status) == PHILO_IS_DEAD)
			p_kill_child_philos(*data);
		else if ((*data)->philo_full_count == (*data)->philos_total)
			p_kill_child_philos(*data);
		else if (WEXITSTATUS(philo_status) == PHILO_IS_FULL)
			(*data)->philo_full_count++;
	}
	if ((*data)->meals_min)
		p_log_meals_report(philos);
	p_util_close_global_semaphores(*data);
	p_util_free_philos(philos);
	p_util_free_data(data);
	// kill(0, SIGINT);
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
	philos = p_init_philos(data);
	if (!data || !philos)
		return (p_util_error_print(ERR_MALLOC));
	else if (p_sim_start(data, philos) == ERROR)
		return (EXIT_FAILURE);
	// else if (p_monitor_start(&thread_monitor, philos) == ERROR)
		// return (EXIT_FAILURE);
	p_sim_end(&data, thread_monitor, philos);
	return (EXIT_SUCCESS);
}
