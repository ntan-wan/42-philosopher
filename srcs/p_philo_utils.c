/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_philo_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 11:23:55 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/18 15:56:36 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void    p_philo_sleep(t_data *data, time_t time_sleep)
{
	time_t	time_wake_up;

	time_wake_up = p_util_get_time_in_ms() + time_sleep;
	while (p_util_get_time_in_ms() < time_wake_up)
	{
		if ()
			break ;
		usleep(100);
	}
}
