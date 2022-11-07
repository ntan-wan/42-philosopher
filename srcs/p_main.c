/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 11:06:25 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/07 21:08:16 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

















/*size_t get_time(void)
{
	static struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}*/

size_t	p_get_milisecond(void)
{
	static struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/*int	is_dead(t_philosopher *philo)
{
	if (!philo->eating && get_time() > philo->threshold)
	{
		ft_msg(philo, philo->threshold, DIED);
		pthread_mutex_lock(&philo->instance->write_mutex);
		pthread_mutex_unlock(&philo->instance->die_mutex);
		return (1);
	}
	return (0);
}*/

int	p_is_dead(t_philo *philo)
{
	if (!philo->eating && p_get_milisecond() > philo->threshold)
	{
		p_print_msg(philo, philo->threshold, DIED);
		pthread_mutex_lock(&philo->main_data->write_mutex);
		pthread_mutex_unlock(&philo->main_data->die_mutex);
		return (1);
	}
	return (0);
}

/*void	ft_msg(t_philosopher *philo, size_t timestamp, int action)
{
	pthread_mutex_lock(&philo->instance->write_mutex);
	if (action == TAKING_FORK)
		printf("%zu %zu has taken a fork\n", timestamp, philo->position);
	else if (action == EATING)
		printf("%zu %zu is eating 🍕\n", timestamp, philo->position);
	else if (action == SLEEPING)
		printf("%zu %zu is sleeping 💤\n", timestamp, philo->position);
	else if (action == THINKING)
		printf("%zu %zu is thinking 💭\n", timestamp, philo->position);
	else if (action == DIED)
		printf("%zu %zu died 💀\n", timestamp, philo->position);
	else if (action == OVER)
		printf("%zu %zu has finished his meals 🤢\n", timestamp, philo->position);
	pthread_mutex_unlock(&philo->instance->write_mutex);
}*/

void	p_print_msg(t_philo *philo, size_t timestamp, int action)
{
	pthread_mutex_lock(&philo->main_data->write_mutex);
	if (action == TAKING_FORK)
		printf("%zu %zu has taken a fork\n", timestamp, philo->position);
	else if (action == EATING)
		printf("%zu %zu is eating\n", timestamp, philo->position);
	else if (action == SLEEPING)
		printf("%zu %zu is sleeping\n", timestamp, philo->position);
	else if (action == THINKING)
		printf("%zu %zu is thinking\n", timestamp, philo->position);
	else if (action == DIED)	
		printf("%zu %zu died\n", timestamp, philo->position);
	else if (action == OVER)
		printf("%zu %zu has finished his meals\n", timestamp, philo->position);
	pthread_mutex_unlock(&philo->main_data->write_mutex);
}

/*int	eat(t_philosopher *philo)
{
	philo->eating = 1;
	philo->last_meal = get_time();
	ft_msg(philo, philo->last_meal, EATING);
	ft_usleep(philo->instance->tteat);
	philo->threshold = philo->last_meal + philo->instance->ttdie;
	philo->eating = 0;
	philo->meals += 1;
	if (philo->instance->meals > 0 && philo->meals >= philo->instance->meals)
	{
		ft_msg(philo, philo->last_meal, OVER);
		pthread_mutex_lock(&philo->instance->frks_mutexes[philo->l_fork]);
		philo->instance->forks[philo->l_fork] = 0;
		pthread_mutex_unlock(&philo->instance->frks_mutexes[philo->l_fork]);
		pthread_mutex_lock(&philo->instance->frks_mutexes[philo->r_fork]);
		philo->instance->finished++;
		if (philo->instance->finished >= philo->instance->amount)
		{
			pthread_mutex_lock(&philo->instance->write_mutex);
			pthread_mutex_unlock(&philo->instance->die_mutex);
		}
		return (1);
	}
	return (0);
}*/

int	p_eat(t_philo *philo)
{
	philo->eating = 1;
	philo->last_meal = p_get_milisecond();
	p_print_msg(philo, philo->last_meal, EATING);
	p_usleep(philo->main_data->time_to_eat);
	philo->threshold = philo->last_meal + philo->main_data->time_to_die;
	philo->eating = 0;
	philo->meals += 1;
	if (philo->main_data->meals > 0 && philo->meals >= philo->main_data->meals)
	{
		p_print_msg(philo, philo->last_meal, OVER);
		pthread_mutex_lock(&philo->main_data->forks_mutex[philo->left_fork]);
		philo->main_data->forks[philo->left_fork] = 0;
		pthread_mutex_unlock(&philo->main_data->forks_mutex[philo->left_fork]);
		pthread_mutex_lock(&philo->main_data->forks_mutex[philo->right_fork]);
		philo->main_data->forks[philo->right_fork] = 0;
		pthread_mutex_unlock(&philo->main_data->forks_mutex[philo->right_fork]);
		philo->main_data->finished++;
		if (philo->main_data->finished >= philo->main_data->philo_total)
		{
			pthread_mutex_lock(&philo->main_data->write_mutex);
			pthread_mutex_unlock(&philo->main_data->die_mutex);
		}
		return (1);
	}
	return (0);
}

/*int	take_forks(t_philosopher *philo)
{
	int		i;
	size_t	fork;

	i = 0;
	while (i < 2)
	{
		if (is_dead(philo))
			return (1);
		if (i == 0)
			fork = philo->l_fork;
		else
			fork = philo->r_fork;
		pthread_mutex_lock(&philo->instance->frks_mutexes[fork]);
		if (!philo->instance->forks[fork])
		{
			ft_msg(philo, get_time(), TAKING_FORK);
			philo->instance->forks[fork] = 1;
			i++;
		}
		pthread_mutex_unlock(&philo->instance->frks_mutexes[fork]);
	}
	return (0);
}*/

int	p_take_forks(t_philo *philo)
{
	int		i;
	size_t	fork;
	
	i = 0;
	while (i < 2)
	{
		if (p_is_dead(philo))
			return (1);
		if (i == 0)
			fork = philo->left_fork;
		else
			fork = philo->right_fork;
		pthread_mutex_lock(&philo->main_data->forks_mutex[fork]);
		if (!philo->main_data->forks[fork])
		{
			p_print_msg(philo, p_get_milisecond(), TAKING_FORK);
			philo->main_data->forks[fork] = 1;
			i++;
		}
		pthread_mutex_unlock(&philo->main_data->forks_mutex[fork]);
	}
	return (0);
}

/*int	release_forks(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->instance->frks_mutexes[philo->l_fork]);
	philo->instance->forks[philo->l_fork] = 0;
	pthread_mutex_unlock(&philo->instance->frks_mutexes[philo->l_fork]);
	pthread_mutex_lock(&philo->instance->frks_mutexes[philo->r_fork]);
	philo->instance->forks[philo->r_fork] = 0;
	pthread_mutex_unlock(&philo->instance->frks_mutexes[philo->r_fork]);
	ft_msg(philo, philo->last_meal + philo->instance->tteat, SLEEPING);
	ft_usleep(philo->instance->ttsleep);
	if (is_dead(philo))
		return (1);
	ft_msg(philo, philo->last_meal + philo->instance->tteat + philo->instance->ttsleep, THINKING);
	return (0);
	
}*/

int	p_release_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->main_data->forks_mutex[philo->left_fork]);
	philo->main_data->forks[philo->left_fork] = 0;
	pthread_mutex_unlock(&philo->main_data->forks_mutex[philo->left_fork]);
	pthread_mutex_lock(&philo->main_data->forks_mutex[philo->right_fork]);
	philo->main_data->forks[philo->right_fork] = 0;
	pthread_mutex_unlock(&philo->main_data->forks_mutex[philo->right_fork]);
	p_print_msg(philo, philo->last_meal + philo->main_data->time_to_eat, SLEEPING);
	p_usleep(philo->main_data->time_to_sleep);
	if (p_is_dead(philo))
		return (1);
	p_print_msg(philo, philo->last_meal + philo->main_data->time_to_eat + philo->main_data->time_to_sleep, THINKING);
	return (0);
}

/*void	*routine(void *vphilo)
{
	t_philosopher *philo;

	philo = (t_philosopher *)vphilo;
	philo->last_meal = get_time();
	philo->threshold = philo->last_meal + philo->instance->ttdie;
	while (1)
	{
		if (take_forks(philo) || eat(philo) || release_forks(philo))
			break ;
	}
	return (NULL);
}*/

void	*p_routine(void *philosopher)
{
	t_philo	*philo;

	philo = (t_philo *)philosopher;
	philo->last_meal = p_get_milisecond();
	philo->threshold = philo->last_meal + philo->main_data->time_to_die;
	while (1)
	{
		if (p_take_forks(philo) || p_eat(philo) || p_release_forks(philo))
			break ;
	}
	return (NULL);
}

/*void	ft_usleep(size_t ms)
{
	size_t end;

	end = get_time() + ms;
	while (get_time() < end)
		usleep(ms / 1000);
}*/

void	p_usleep(size_t ms)
{
	size_t	end;

	end = p_get_milisecond() + ms;
	while (p_get_milisecond() < end)
		usleep(ms / 1000);
}

/*int	ft_log(int	error)
{
	if (error == WRONG_ARGUMENT)
		printf("Error: Wrong argument.\n");
	else if (error == TOO_MANY_ARGUMENTS)
		printf("Error: Wrong syntax.\n");
	else if (error == MALLOC_ERROR)
		printf("Error: Malloc error.\n");
	else if (error == PTHREAD_ERROR)
		printf("Error: PThread error.\n");
	return (1);
}*/

/*int	start(t_main *instance)
{
	size_t		i;
	pthread_t	tid;

	i = 0;
	pthread_mutex_lock(&instance->die_mutex);
	while (i < instance->amount)
	{
		if (pthread_create(&tid, NULL, routine, (void *)&instance->philosophers[i]))
			return (ft_log(PTHREAD_ERROR));
		pthread_detach(tid);
		if (instance->amount % 2)
			ft_usleep(500);
		i++;
	}
	return (0);
}*/

void	p_philo_do_routine(t_data *data)
{
	size_t		i;
	pthread_t	tid;

	i = 0;
	pthread_mutex_lock(&data->die_mutex);
	while (i < data->philo_total)
	{
		pthread_create(&tid, NULL, p_routine, (void *)&data->philosophers[i]);
		pthread_detach(tid);
		if (data->philo_total % 2)
			p_usleep(500);
		i++;
	}
}


/*int	clear(t_main *instance)
{
	size_t i;
	
	i = 0;
	while (i < instance->amount)
	{
		pthread_mutex_destroy(&instance->frks_mutexes[i]);
		i++;
	}
	pthread_mutex_destroy(&instance->write_mutex);
	pthread_mutex_destroy(&instance->die_mutex);
	free(instance->frks_mutexes);
	free(instance->philosophers);
	free(instance->forks);
	return (0);
}*/

int	main(int ac, char **av)
{
	t_data	data;

	(void)ac;
	if (!is_valid_input(ac, av))
		return (ERROR);
	p_input_parse(&data, av);
	p_init_philos(&data);
	p_init_forks(&data);
	p_init_mutexes(&data);
	p_philo_do_routine(&data);
	pthread_mutex_lock(&data.die_mutex);
	pthread_mutex_unlock(&data.die_mutex);
	return (0);
}
