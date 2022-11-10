/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_input_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 21:10:22 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/09 20:15:46 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static size_t	p_atoi_unsigned_int(const char *str)
{
	int		i;
	size_t	num;

	i = -1;
	num = 0;
	while (str[++i] && str[i] >= '0' && str[i] <= '9')
		num = num * 10 + (str[i] - '0');
	return (num);
}

static int	p_is_invalid_str_num(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (ERROR);
	}
	return (VALID);
}

static int p_error(int error)
{
	if (error == INVALID_NUM_ARGS)
		printf("error: invalid num of args\n");
	else if (error == WRONG_ARGUMENT)
		printf("error: wrong argument\n");
	else if (error == INVALID_CHAR)
		printf("error: invalid char");
	return (1);
}

int p_is_invalid_input(int ac, char **av)
{
	int		i;
	int		j;
	char	*str;

	if (ac < 5 || ac > 6)
		return (p_error(INVALID_NUM_ARGS));
	i = -1;
	str = "\0";
	while (av[++i])
	{
		str = av[i];
		if (str[0] == '\0')
			return (WRONG_ARGUMENT);
		else if (p_atoi_unsigned_int(str) == 0)
			return (p_error(WRONG_ARGUMENT));
		else if (p_is_invalid_str_num(str))
			return (p_error(INVALID_CHAR));
	}
	return (VALID);
}

void	p_input_parse(t_data *data, char **av)
{
	if (av[5])
	{
		data->meals_minimum = p_atoi_unsigned_int(av[5]);
		data->meals_total_all = data->meals_minimum * data->philo_total;
	}
	else
	{
		data->meals_minimum = 0;
		data->meals_total_all = 0;
	}
	data->meals_total_all_eaten = 0;
	data->philo_total = p_atoi_unsigned_int(av[1]);
	data->time_to_die = p_atoi_unsigned_int(av[2]);
	data->time_to_eat = p_atoi_unsigned_int(av[3]);
	data->time_to_sleep = p_atoi_unsigned_int(av[4]);
	data->philo_died = 0;
}
