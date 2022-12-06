/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_other_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntan-wan <ntan-wan@42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 07:02:08 by ntan-wan          #+#    #+#             */
/*   Updated: 2022/12/06 19:10:21 by ntan-wan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	p_util_strlen(char *str)
{
	int	str_len;

	str_len = 0;
	while (*str++)
		str_len++;
	return (str_len);
}

void	p_util_strcat(char *dst, char *src)
{
	if (!dst || !src)
		return ;
	while (*dst != '\0')
		dst++;
	while (*src)
		*dst++ = *src++;
}

int	p_util_digit_count(unsigned int num)
{
	int	digit_count;

	digit_count = 0;
	if (num == 0)
		digit_count = 1;
	while (num)
	{
		num /= 10;
		digit_count++;
	}
	return (digit_count);
}

char	*p_util_utoa(unsigned int num)
{
	char	*str_num;
	int		str_len;

	str_len = p_util_digit_count(num);
	str_num = (char *)malloc(sizeof(char) * (str_len + 1));
	if (!str_num)
		return (NULL);
	str_num[0] = '0';
	str_num[str_len] = '\0';
	while (num % 10)
	{
		str_num[--str_len] = num % 10 + '0';
		num /= 10;
	}
	return (str_num);
}

void	*p_util_calloc(size_t count, size_t size)
{
	size_t			i;
	unsigned char	*mem;

	i = 0;
	mem = (unsigned char *)malloc(count * size);
	while (i < count)
		mem[i++] = '\0';
	return (mem);
}
