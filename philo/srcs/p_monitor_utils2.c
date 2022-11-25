/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_monitor_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 19:23:34 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/25 19:34:15 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	p_monitor_delay(t_philo **philos)
{
	unsigned int	i;
	t_data			*data;

	data = philos[0]->data;
	i = 0;
	while (i < data->philos_total)
		if (philos[i]->time_last_meal)
			i++;
}
