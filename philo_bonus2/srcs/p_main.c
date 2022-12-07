/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:05:16 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/12/07 17:33:31 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

static void	p_sim_start(t_data *data, t_philo **philos)
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
}

static void	p_philo_status_check(t_data *data)
{
	int				philo_status;
	pthread_t		philo_is_full_check;

	philo_status = 0;
	pthread_create(&philo_is_full_check, NULL, p_monitor_is_full_check, data);
	while (waitpid(-1, &philo_status, 0) != -1)
	{
		if (WEXITSTATUS(philo_status) == PHILO_IS_DEAD)
		{
			data->sim_stop = true;
			p_util_kill_all_philos(data);
		}
	}
	pthread_join(philo_is_full_check, NULL);
}

static void	p_sim_end(t_data **data, t_philo **philos)
{
	p_free_global_sems(*data);
	p_util_free_philos(philos);
	p_util_free_data(data);
}

int	main(int ac, char **av)
{
	t_data		*data;
	t_philo		**philos;

	data = NULL;
	if (is_invalid_input(ac, av))
		return (EXIT_FAILURE);
	data = p_init_data(av);
	philos = p_init_philos(data);
	if (!data || !philos)
		return (EXIT_FAILURE);
	p_sim_start(data, philos);
	p_philo_status_check(data);
	p_sim_end(&data, philos);
	return (EXIT_SUCCESS);
}
