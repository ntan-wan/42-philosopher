/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 11:07:49 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/21 15:21:44 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>
# include <limits.h>

# define SUCCESS 0
# define ERROR 1

# define RED	"\e[31m"
# define GREEN	"\e[32m"
# define PURPLE	"\e[35m"
# define YELLOW	"\e[33m"
# define COLOR_OFF	"\e[0m"

# define ERR_MALLOC	"Could not allocate memory"
# define ERR_MUTEX	"Could not create mutex"
# define ERR_THREAD "Could not create thread"

typedef enum e_status
{
	DIED,
	EATING,
	SLEEPING,
	THINKING,
	GET_FORK_1,
	GET_FORK_2
}	t_status;

typedef struct s_data
{
	bool			sim_stop;
	unsigned int	meals_minimum;
	time_t			time_to_die;
	time_t			time_start;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	unsigned int	philos_total;
	// 
	// pthread_t		monitor_philo;
	pthread_mutex_t	lock_log;
	pthread_mutex_t	lock_sim_stop;
	pthread_mutex_t	*locks_forks;
	//
	// struct s_philo	**philos;
}	t_data;

typedef struct s_philo
{
	pthread_t		thread;
	unsigned int	fork[2];
	unsigned int	meals_count;
	unsigned int	id;
	time_t			time_last_meal;
	pthread_mutex_t	lock_meal_time;
	t_data			*data;
}	t_philo;

/* input_check_utils */
int		is_invalid_input(int ac, char **av);

/* init_utils */
t_data	*p_init_data(char **av);
t_philo **p_init_philos(t_data *data);

/* philo_utils */
void	p_philo_eats(t_philo *philo);
void	p_philo_thinks(t_philo *philo);
void	p_philo_sleeps(t_philo *philo);
void	p_philo_get_forks(t_philo *philo);
void	p_philo_release_forks(t_philo *philo);

/* routine_utils */
void    *p_routine_philo(void *philosopher);

/* monitor_utils */
int	p_monitor_start(pthread_t *thread_monitor, t_philo **philos);

/* monitor_utils2 */
bool	p_monitor_sim_has_stopped(t_data *data);

/* log_utils */
void    p_log_status(t_philo * philo, t_status status);

/* other_utils */
int		p_util_atoi(char *str);
time_t	p_util_get_time_in_ms(void);
int 	p_util_error_print(char *msg);
void    p_util_delay(time_t start_time);
void    p_util_usleep(t_data *data, time_t time_sleep);

/* other_utils2 */
void	p_util_destroy_mutexes(t_data *data, t_philo **philos);
void	p_util_free_data_and_philos(t_data **all_data, t_philo **philos);
#endif