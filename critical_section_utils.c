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

int get_sync_data(t_critical_section *cs, void *addr, unsigned int size)
{
	if (pthread_mutex_lock(cs->mutex))
		return prnt_err("get_sync_data: mutex could not locked"), BAD_PHILO_EXIT;
	ft_memcpy(addr, cs->data, size);
	if (pthread_mutex_unlock(cs->mutex))
		return prnt_err("get_sync_data: mutex could not unlocked"), BAD_PHILO_EXIT;
	return GOOD_PHILO_EXIT;
}

int set_sync_data(t_critical_section *cs, void *data, unsigned int size)
{
	if (pthread_mutex_lock(cs->mutex))
		return prnt_err("set_sync_data: mutex could not locked"), BAD_PHILO_EXIT;
	ft_memcpy(cs->data, data, size);
	if (pthread_mutex_unlock(cs->mutex))
		return prnt_err("set_sync_data: mutex could not unlocked"),BAD_PHILO_EXIT;
	return GOOD_PHILO_EXIT;
}

int create_cs(t_critical_section *addr, unsigned int size)
{
	t_critical_section cs;

	cs.mutex = create_lock();
	if (!cs.mutex)
		return prnt_err("create_cs: mutex could not created"), BAD_PHILO_EXIT;
	cs.data = malloc(size);
	if (!cs.data)
		return (abort_mutex(&cs.mutex), BAD_PHILO_EXIT);
	*addr = cs;
	return GOOD_PHILO_EXIT;
}

void abort_critical_section(t_critical_section cs)
{
	if (cs.mutex)
		abort_mutex(&cs.mutex);
	if (cs.data)
		free(cs.data);
}