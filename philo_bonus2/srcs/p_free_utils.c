/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_free_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 19:09:52 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/12/06 19:12:30 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	p_util_free_data(t_data **all_data)
{
	t_data	*data;

	data = *all_data;
	if (!data)
		return ;
	free(data->pids);
	free(data);
	*all_data = NULL;
}

void	p_util_free_philos(t_philo **philos)
{
	int		i;
	t_data	*data;

	i = -1;
	data = philos[0]->data;
	while (philos && ++i < (int)data->philos_total)
	{
		if (philos[i])
		{
			sem_close(philos[i]->sem_meal);
			free(philos[i]);
		}
	}
	free(philos);
}

void	p_free_global_sems(t_data *data)
{
	sem_close(data->sem_forks);
	sem_unlink(SEM_NAME_FORKS);
	sem_close(data->sem_log);
	sem_unlink(SEM_NAME_LOG);
	sem_close(data->sem_sim_stop);
	sem_unlink(SEM_NAME_STOP);
}
