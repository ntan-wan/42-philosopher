/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 11:07:49 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/08 12:25:32 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

# define VALID 0
# define ERROR 1
# define INVALID_NUM_ARGS -1
# define WRONG_ARGUMENT -2
# define INVALID_CHAR -3
// # define WRONG_ARGUMENT -1
// # define TOO_MANY_ARGUMENTS -2
// # define MALLOC_ERROR -3
// # define PTHREAD_ERROR -4

typedef enum e_action
{
	TAKING_FORK,
	EATING,
	SLEEPING,
	THINKING,
	DIED,
	OVER,
}				t_action;

typedef struct s_philo
{
	size_t			position;
	size_t			left_fork;
	size_t			right_fork;
	size_t			meals;
	size_t			eating;
	size_t			last_meal;
	size_t			threshold;
	struct	s_data	*base_data;
}	t_philo;

typedef	struct s_data
{
	size_t			philo_total;
	size_t			time_to_eat;
	size_t			time_to_die;
	size_t			time_to_sleep;
	size_t			meals;
	size_t			finished;
	t_philo			*philos;
	int				*forks;
	pthread_mutex_t	die_mutex;
	pthread_mutex_t	write_mutex;
	pthread_mutex_t	*forks_mutex;
}	t_data;

void	p_util_usleep(size_t ms);

/* init_utils */
void	p_init_forks(t_data *data);
void	p_init_philos(t_data *data);
void	p_init_mutexes(t_data *data);

/* input_utils */
int 	p_is_invalid_input(int ac, char **av);
size_t	p_atoi_unsigned_int(const char *str);
void	p_input_parse(t_data *data, char **av);

/* philo_utils */
void	p_routine_start(t_data *data);

/* other_utils */
size_t	p_util_get_milisecond(void);
void	p_util_log(size_t timestamp, t_philo *philo, int action);
#endif