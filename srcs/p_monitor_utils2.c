/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_monitor_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 10:54:14 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/19 14:44:48 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	p_monitor_sim_has_stopped(t_data *data)
{
	bool	result;

	pthread_mutex_lock(&data->lock_sim_stop);
	result = data->sim_stop;
	pthread_mutex_unlock(&data->lock_sim_stop);
	return (result);
}
