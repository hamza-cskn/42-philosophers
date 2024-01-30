/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcoskun <hcoskun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 19:15:01 by hcoskun           #+#    #+#             */
/*   Updated: 2024/01/30 19:15:35 by hcoskun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "philo.h"

long long	unsigned_atoi(const char *str)
{
	long long	res;

	res = 0;
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + *str - '0';
		str++;
	}
	if (*str)
		return (-1);
	return (res);
}

pthread_mutex_t	*create_lock(void)
{
	pthread_mutex_t	*mutex;

	mutex = malloc(sizeof(pthread_mutex_t));
	if (!mutex)
		return (NULL);
	if (pthread_mutex_init(mutex, NULL))
		return (NULL);
	return (mutex);
}

void	*ft_calloc(unsigned long size, int count)
{
	void	*block;

	block = malloc(size * count);
	if (!block)
		return (NULL);
	memset(block, 0, size * count);
	return (block);
}
