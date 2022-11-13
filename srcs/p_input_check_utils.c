/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_input_check_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 11:32:15 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/11/12 21:05:53 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int	is_not_num(char *str)
{
	if (*str == '-' || *str == '+')
	{
		if (*(str + 1) < '0' || *(str + 1) > '9')
			return (ERROR);
		str++;
	}
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (ERROR);
		str++;
	}
	return (VALID);
}

static int	is_less_than_60ms(char *num, size_t i)
{
	return ((i >= 2 && i <= 4) && (p_util_a_to_unsigned_int(num) < 60));
}

int	is_invalid_input(int ac, char **av)
{
	size_t	i;

	i = 0;
	if (ac != 5 && ac != 6)
		return (p_util_error_print("err: invalid num of args"));
	while (av[++i])
	{
		if (is_not_num(av[i]))
			return (p_util_error_print("err: is not a num"));
		else if (av[i][0] == '-')
			return (p_util_error_print("err: negative num not allowed"));
		else if (is_less_than_60ms(av[i], i))
			return (p_util_error_print("err: less than 60ms"));
	}
	return (VALID);
}
