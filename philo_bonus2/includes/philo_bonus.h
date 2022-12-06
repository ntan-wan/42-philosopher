/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 11:07:49 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/12/06 19:55:15 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>
# include <limits.h>
# include <semaphore.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>

# define SUCCESS 0
# define ERROR 1

# define PHILO_IS_DEAD 100
# define PHILO_IS_FULL 200

# define BOLD "\e[1m"
# define UNDERLINE "\x1b[4m"
# define BOLD_AND_RED_BG "\33[1;101m"
# define BOLD_AND_GREEN_BG "\33[1;30;42m"
# define RED	"\e[31m"
# define GREEN	"\e[32m"
# define PURPLE	"\e[35m"
# define YELLOW	"\e[33m"
# define COLOR_OFF	"\e[0m"

# define ERR_MALLOC	"Could not allocate memory"
# define ERR_MUTEX	"Could not create mutex"
# define ERR_THREAD "Could not create thread"

# define SEM_NAME_FORKS "sem_global_forks"
# define SEM_NAME_LOG "sem_global_log"
# define SEM_NAME_STOP "sem_global_stop"
# define SEM_NAME_MEAL "sem_local_meal"

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
	time_t			time_to_die;
	time_t			time_start;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	unsigned int	philos_total;
	unsigned int	meals_min;
	sem_t			*sem_log;
	sem_t			*sem_forks;
	sem_t			*sem_sim_stop;
	pid_t			*pids;
}	t_data;

typedef struct s_philo
{
	unsigned int	id;
	unsigned int	meals_count;
	time_t			time_last_meal;
	sem_t			*sem_meal;
	t_data			*data;
	pthread_t		death_check;
}	t_philo;

/* input_check_utils */
int		is_invalid_input(int ac, char **av);

/* init_utils */
t_data	*p_init_data(char **av);
t_philo	**p_init_philos(t_data *data);

/* philo_utils */
void	p_philo_eats(t_philo *philo);
void	p_philo_thinks(t_philo *philo);
void	p_philo_sleeps(t_philo *philo);
void	p_philo_get_forks(t_philo *philo);
void	p_philo_release_forks(t_philo *philo);

/* routine_utils */
void	*p_routine_philo(void *philosopher);

/* monitor_utils */
void	*p_monitor_death(void *philosopher);
int		p_monitor_philo_is_full(t_philo *philo);
bool	p_monitor_sim_has_stopped(t_data *data);
void	p_monitor_set_sim_stop(t_data *data, bool state);

/* log_utils */
void	p_log_meals_report(t_philo **philos);
void	p_log_status(t_philo *philo, t_status status);
void	p_log_death_report(time_t time_current, t_philo *philo);

/* other_utils */
int		p_util_atoi(char *str);
time_t	p_util_get_time_in_ms(void);
void	p_util_delay(time_t time_start);
void	p_util_kill_philos(t_data *data);
void	p_util_usleep_sim_check(t_data *data, time_t time_sleep);

/* other_utils2 */
int		p_util_strlen(char *str);
char	*p_util_utoa(unsigned int num);
void	p_util_strcat(char *dst, char *src);
int		p_util_digit_count(unsigned int num);
void	*p_util_calloc(size_t count, size_t size);

/* free_utils */
void	p_free_global_sems(t_data *data);
void	p_util_free_data(t_data **all_data);
void	p_util_free_philos(t_philo **philos);
#endif