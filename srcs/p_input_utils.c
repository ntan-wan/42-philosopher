/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_input_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 21:10:22 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/07 21:10:24 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static size_t	ft_atoi_unsigned_int(const char *str)
{
	int		i;
	size_t	num;

	i = -1;
	num = 0;
	while (str[++i] && str[i] >= '0' && str[i] <= '9')
		num = num * 10 + (str[i] - '0');
	return (num);
}

static int	is_valid_str_num(char *str)
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


int is_valid_input(int ac, char **av)
{
	int		i;
	int		j;
	char	*str;

	if (ac < 5 || ac > 6)
		return (ERROR);
	i = -1;
	str = "\0";
	while (av[++i])
	{
		str = av[i];
		if (str[0] == '\0')
			return (ERROR);
		else if (ft_atoi_unsigned_int(str) == 0)
			return (ERROR);
		else if (!is_valid_str_num(str))
			return (ERROR);
	}
	return (VALID);
}


void	p_input_parse(t_data *data, char **av)
{
	if (av[5])
		data->meals = ft_atoi_unsigned_int(av[5]);
	else
		data->meals = -1;
	data->philo_total = ft_atoi_unsigned_int(av[1]);
	data->time_to_die = ft_atoi_unsigned_int(av[2]);
	data->time_to_eat = ft_atoi_unsigned_int(av[3]);
	data->time_to_sleep = ft_atoi_unsigned_int(av[4]);
	data->finished = 0;
}