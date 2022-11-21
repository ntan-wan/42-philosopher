/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:05:16 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/21 15:19:38 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int	p_sim_start(t_data *data, t_philo **philos)
{
	unsigned int	i;

	data->time_start = p_util_get_time_in_ms() + (data->philos_total * 2 * 10);
	i = 0;
	while (i < data->philos_total)
	{
		if (pthread_create(&philos[i]->thread, NULL, p_routine_philo, philos[i]) != 0)
			return (p_util_error_print(ERR_THREAD));
		i++;
	}
	return (SUCCESS);
}

static void	p_sim_end(t_data **data, pthread_t thread_monitor, t_philo **philos)
{
    unsigned int    i;

    i = 0;
    while (i < (*data)->philos_total)
    {
        pthread_join(philos[i]->thread, NULL);
        i++;
    }
    if ((*data)->philos_total > 1)
        pthread_join(thread_monitor, NULL);
    p_util_destroy_mutexes(*data, philos);
	p_util_free_data_and_philos(data, philos);
}

int main(int ac, char **av)
{
	t_data		*data;
	t_philo		**philos;
	pthread_t	thread_monitor;

	data = NULL;
	if (is_invalid_input(ac, av))
		return (EXIT_FAILURE);
	data = p_init_data(av);
	philos = p_init_philos(data);
	if (!data)
		return (EXIT_FAILURE);
	else if (p_sim_start(data, philos) == ERROR)
		return (EXIT_FAILURE);
	else if (p_monitor_start(&thread_monitor, philos) == ERROR)
		return (EXIT_FAILURE);
	p_sim_end(&data, thread_monitor, philos);
	return (EXIT_SUCCESS);
}