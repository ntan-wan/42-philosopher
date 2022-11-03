/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 11:06:25 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/03 16:10:16 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

time_t p_get_millisecond(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));

}

pthread_mutex_t **p_init_forks(int forks_total)
{
	int	i;
	pthread_mutex_t	**forks;

	forks = (pthread_mutex_t **)malloc(sizeof(pthread_mutex_t *) * (forks_total + 1));
	if (!forks)
		return (NULL);
	forks[forks_total] = NULL;
	i = -1;
	while (++i < forks_total)
		pthread_mutex_init(forks[i], NULL);
	return (forks);
}

int main(int ac, char **av)
{
	(void)ac;
	(void)av;

	printf("%ld\n", p_get_millisecond());
	return (0);
}
