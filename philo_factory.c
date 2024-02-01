/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_factory.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcoskun <hcoskun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 19:08:30 by hcoskun           #+#    #+#             */
/*   Updated: 2024/02/01 14:26:50 by hcoskun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include "philo.h"
#include <stdlib.h>

/**
 * Frees all pointers in a philosopher except sticks.
 * @param philo
 */
void	free_philosopher(t_philosopher philo)
{
	abort_critical_section(philo.last_eat_time_cs);
	abort_critical_section(philo.eat_count_cs);
	abort_critical_section(philo.state_cs);
}

void	free_philosophers(t_philosopher *philos, int count)
{
	int	i;

	if (!philos)
		return ;
	i = 0;
	while (i < count)
	{
		free_philosopher(philos[i]);
		i++;
	}
	safe_free(philos);
}

pthread_mutex_t	**create_sticks(int amount)
{
	pthread_mutex_t	**sticks;
	int				i;

	sticks = malloc(sizeof(pthread_mutex_t *) * amount);
	if (!sticks)
		return (NULL);
	i = 0;
	while (i < amount)
	{
		sticks[i] = create_lock();
		if (!sticks[i])
			return (abort_sticks(sticks, i), NULL);
		i++;
	}
	return (sticks);
}

int	create_philosopher(t_philosopher *addr, unsigned int id, t_simulation *sim)
{
	t_philosopher	philo;

	philo.id = id;
	philo.simulation = sim;
	if (create_cs(&philo.last_eat_time_cs, sizeof(struct timeval)))
		return (BAD_PHILO_EXIT);
	if (create_cs(&philo.state_cs, sizeof(t_philo_state)))
		return (abort_critical_section(philo.last_eat_time_cs), BAD_PHILO_EXIT);
	if (create_cs(&philo.eat_count_cs, sizeof(int)))
	{
		abort_critical_section(philo.last_eat_time_cs);
		abort_critical_section(philo.state_cs);
		return (BAD_PHILO_EXIT);
	}
	*((t_time *) philo.last_eat_time_cs.data) = (t_time){2147483647, 0};
	*((t_philo_state *) philo.state_cs.data) = THINKING;
	*((int *) philo.eat_count_cs.data) = 0;
	*addr = philo;
	return (GOOD_PHILO_EXIT);
}

/**
 * creates philosophers array and initialize philosopher structs.
 * 
 * in an error state,
 * frees all pointers and returns null cleanly.
*/
t_philosopher	*create_philosophers(t_simulation *simulation)
{
	int				i;
	t_philosopher	*philos;

	philos = ft_calloc(sizeof(t_philosopher), (int) simulation->philo_count);
	i = 0;
	while (i < (int) simulation->philo_count)
	{
		if (create_philosopher(&philos[i], i, simulation))
			return (free_philosophers(philos, i - 1), NULL);
		i++;
	}
	return (philos);
}
