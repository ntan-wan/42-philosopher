/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 11:07:49 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/14 11:24:21 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

/*
typedef struct s_philo
{
	pthread_mutex_t	*eat;
	size_t			eating;
	size_t			position;
	size_t			fork_left;
	size_t			fork_right;
	size_t			meals_count;
	size_t			time_last_meal;
	size_t			time_limit_to_eat;
	struct	s_data	*base_data;
}	t_philo;

typedef	struct s_data
{
	t_philo			*philos;
	size_t			philo_died;
	size_t			philo_total;
	size_t			philos_finished_count;
	size_t			time_to_eat;
	size_t			time_to_die;
	size_t			time_to_sleep;
	size_t			meals_minimum;
	// size_t			meals_total_all_eaten;
	// size_t			meals_total_all;
	pthread_mutex_t	*mutex_forks;
	pthread_mutex_t	mutex_log;
	pthread_mutex_t	mutex_routine_end;
}	t_data;
*/
typedef enum e_validity
{
	VALID,
	ERROR
}	t_validity;

typedef enum e_action
{
	TAKING_FORK,
	RELEASE_FORK,
	EATING,
	SLEEP,
	THINK,
	DIED
}	t_action;

typedef struct	s_data
{
	size_t			philos_total;
	size_t			time_to_eat;
	size_t			time_to_die;
	size_t			time_to_sleep;
	size_t			time_routine_start;
	size_t			meals_minimum;
	pthread_mutex_t	mutex_die;
	pthread_mutex_t	*mutex_forks;
	pthread_mutex_t	mutex_simulation_end;
}	t_data;

typedef struct	s_philo
{
	pthread_t	pid;
	size_t		fork_left;
	size_t		fork_right;
	size_t		meals_count;
	size_t		time_last_meal;
	size_t		time_limit_to_eat;
	size_t		position_num;
	t_data		*data;
}	t_philo;

/* init_utils */
void	p_init_mutexes(t_data *data);
void	p_init_philos(t_data *data, t_philo **philos);

/* input_check_utils */
int	is_invalid_input(int ac, char **av);

/* routine */
void	p_routine_start(t_data *data, t_philo *philos);

/* action_utils */
void    p_action_eat(t_philo *philo);
void    p_action_sleep(t_philo *philo);
void    p_action_take_forks(t_philo *philo);
void    p_action_thinking(t_philo *philo);

/* other_utils */
size_t	p_util_get_millisecond(void);
size_t	p_util_get_time(t_data *data);
int		p_util_error_print(char *msg);
size_t  p_util_a_to_unsigned_int(char *str);
void	p_util_log(size_t timestamp, t_philo *philo, int action);


#endif