/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_input_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 18:57:49 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/12/06 19:01:09 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"
#define VALID 0
#define INVALID 1

static int	p_error_print(char *msg)
{
	printf("err: %s\n", msg);
	return (ERROR);
}

static int	is_not_num(char *str)
{
	if (*str == '-' || *str == '+')
	{
		if (*(str + 1) < '0' || *(str + 1) > '9')
			return (INVALID);
		str++;
	}
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (INVALID);
		str++;
	}
	return (VALID);
}

static int	is_less_than_60ms(char *num, size_t i)
{
	return ((i >= 2 && i <= 4) && (p_util_atoi(num) < 60));
}

int	is_invalid_input(int ac, char **av)
{
	size_t	i;

	i = 0;
	if (ac != 5 && ac != 6)
		return (p_error_print("invalid num of args"));
	while (av[++i])
	{
		if (is_not_num(av[i]))
			return (p_error_print("is not a num"));
		else if (av[i][0] == '-')
			return (p_error_print("num must be positive"));
		else if (p_util_atoi(av[i]) == 0)
			return (p_error_print("num must be greater than 0"));
		else if (is_less_than_60ms(av[i], i))
			return (p_error_print("num must be greater than 60ms"));
	}
	return (VALID);
}
