/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 16:41:19 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/12/04 00:24:30 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <limits.h>
# include <fcntl.h>
# include <semaphore.h>
# include <signal.h>

# define SUCCESS 0
# define ERROR 1

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
# define ERR_SEM	"Could not create semaphore"
# define ERR_THREAD "Could not create thread"

# define SEM_NAME_FORKS "/sem_global_forks"
# define SEM_NAME_SIM "/sem_global_sim"
# define SEM_NAME_LOG "/sem_global_log"
# define SEM_NAME_MEAL "/sem_local_meal"
# define SEM_NAME_FULL	"/philo_global_full"
# define SEM_NAME_DEAD	"/philo_global_dead"

# define CHILD_EXIT_ERR_PTHREAD	40
# define CHILD_EXIT_ERR_SEM		41
# define CHILD_EXIT_PHILO_FULL	42
# define CHILD_EXIT_PHILO_DEAD	43

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
	/*bool			sim_stop;
	time_t			time_to_die;
	time_t			time_start;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	pthread_mutex_t	lock_log;
	pthread_mutex_t	lock_sim_stop;
	pthread_mutex_t	*locks_forks;*/
	pid_t			*pids;
	sem_t			*sem_log;
	sem_t			*sem_forks;
	sem_t			*sem_philo_full;
	sem_t			*sem_philo_dead;
	bool			sim_stop;
	sem_t			*sem_sim_stop;
	time_t			time_start;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	unsigned int	philos_total;
	unsigned int	meals_minimum;
	struct s_philo	*this_philo;
	unsigned int	philo_full_count;
	pthread_t		gluttony_reaper;
	pthread_t		famine_reaper;
	struct s_philo	**philos;
}	t_data;

typedef struct s_philo
{
	unsigned int	id;
	unsigned int	meals_count;
	time_t			time_last_meal;
	sem_t			*sem_log;
	sem_t			*sem_forks;
	sem_t			*sem_philo_full;
	sem_t			*sem_philo_dead;
	sem_t			*sem_meal;
	char			*sem_meal_name;
	pthread_t		death_check;
	t_data			*data;
	bool			ate_enough;
}	t_philo;

/* input_check_utils */
int		is_invalid_input(int ac, char **av);

/* init_utils */
t_data	*p_init_data(char **av);
pid_t	*p_init_pids(t_data *data);
t_philo	**p_init_philos(t_data *data);

/* philo_utils */
void	p_philo_eats(t_philo *philo);
void	p_philo_thinks(t_philo *philo);
void	p_philo_sleeps(t_philo *philo);
void	p_philo_get_forks(t_philo *philo);
void	p_philo_release_forks(t_philo *philo);

/* routine_utils */
void	p_routine_philo(t_data *data);

/* monitor_utils */
void	*p_monitor_philo(void *philosophers);
bool	p_monitor_sim_has_stopped(t_data *data);

/* log_utils */
void	p_log_meals_report(t_philo **philos);
void	p_log_status(t_philo *philo, t_status status);
void	p_log_death_report(time_t time_current, t_philo *philo);

/* other_utils */
int		p_util_atoi(char *str);
time_t	p_util_get_time_in_ms(void);
int		p_util_error_print(char *msg);
void	p_util_delay(time_t time_start);
void	p_util_usleep_sim_check(t_data *data, time_t time_sleep);

/* other_utils2 */
void	p_util_free_data(t_data **all_data);
void	p_util_free_philos(t_philo **philos);
void	p_util_destroy_mutexes(t_data *data, t_philo **philos);
void	p_util_unlink_global_sems(void);

int		p_util_digit_count(unsigned int num);
int		p_util_strlen(char *str);
void	p_util_strcat(char *dst, char *src);
char	*p_util_utoa(unsigned int num);

/* ipc_utils */
void    p_ipc_philo_init(t_data *data, t_philo *philo);



void	p_monitor_set_sim_stop(t_data *data, bool state);
#endif