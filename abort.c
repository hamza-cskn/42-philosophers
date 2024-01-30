/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   abort.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcoskun <hcoskun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 19:03:52 by hcoskun           #+#    #+#             */
/*   Updated: 2024/01/30 19:04:22 by hcoskun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "philo.h"

/**
 * This function is accepts null pointers and frees them
 * if they are not null. Before every free operation, the
 * pointer is set to zero and the memory is zeroed out.
 *
 * @param ptr
 */
void	safe_free(void *ptr)
{
	if (ptr)
		free(ptr);
}

void	abort_mutex(pthread_mutex_t **mutex)
{
	if (!mutex || !*mutex)
		return ;
	pthread_mutex_destroy(*mutex);
	free(*mutex);
	*mutex = NULL;
}

void	abort_sticks(pthread_mutex_t **sticks, int count)
{
	int	i;

	i = 0;
	while (i < count)
		abort_mutex(&sticks[i++]);
	safe_free(sticks);
}

void	abort_simulation(t_simulation *simulation)
{
	unsigned int	i;

	i = 0;
	free_philosophers(simulation->philos, simulation->philo_count);
	abort_sticks(simulation->sticks, simulation->philo_count);
	abort_critical_section(simulation->state_cs);
}
