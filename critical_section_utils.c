/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   critical_section_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcoskun <hcoskun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 19:05:15 by hcoskun           #+#    #+#             */
/*   Updated: 2024/01/30 19:25:24 by hcoskun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <pthread.h>
#include "philo.h"
#include <stdio.h>

void	*ft_memcpy(void *dst, const void *src, unsigned int n)
{
	size_t				i;
	unsigned char		*dest;
	const unsigned char	*source;

	dest = dst;
	source = src;
	if (!dest && !source)
		return (dest);
	if (!dest || !source)
		return (NULL);
	i = 0;
	while (i < n)
	{
		dest[i] = source[i];
		i++;
	}
	return (dest);
}

int	get_sync_data(t_critical_section *cs, void *addr, unsigned int size)
{
	if (pthread_mutex_lock(cs->mutex))
		return (printf("get_sync_data: could not locked\n"), BAD_PHILO_EXIT);
	ft_memcpy(addr, cs->data, size);
	if (pthread_mutex_unlock(cs->mutex))
		return (printf("get_sync_data: could not unlocked\n"), BAD_PHILO_EXIT);
	return (GOOD_PHILO_EXIT);
}

int	set_sync_data(t_critical_section *cs, void *data, int size)
{
	if (pthread_mutex_lock(cs->mutex))
		return (printf("set_sync_data: could not locked\n"), BAD_PHILO_EXIT);
	ft_memcpy(cs->data, data, size);
	if (pthread_mutex_unlock(cs->mutex))
		return (printf("set_sync_data: could not unlocked\n"), BAD_PHILO_EXIT);
	return (GOOD_PHILO_EXIT);
}

int	create_cs(t_critical_section *addr, int size)
{
	t_critical_section	cs;

	cs.mutex = create_lock();
	if (!cs.mutex)
		return (printf("create_cs: mutex could not created\n"), BAD_PHILO_EXIT);
	cs.data = malloc(size);
	if (!cs.data)
		return (abort_mutex(&cs.mutex), BAD_PHILO_EXIT);
	*addr = cs;
	return (GOOD_PHILO_EXIT);
}

void	abort_critical_section(t_critical_section cs)
{
	if (cs.mutex)
		abort_mutex(&cs.mutex);
	if (cs.data)
		free(cs.data);
}
