/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watchdog_thread.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcoskun <hcoskun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 19:21:46 by hcoskun           #+#    #+#             */
/*   Updated: 2024/01/30 19:31:13 by hcoskun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"
#include <unistd.h>

/**
 * Compares given timeval to each other
 *
 * @param a first timeval
 * @param b second timeval
 * @return 1 if a is bigger than b, 0 otherwise
 */
int	is_bigger_than(t_time a, t_time b)
{
	if (a.tv_sec > b.tv_sec)
		return (1);
	if (a.tv_sec < b.tv_sec)
		return (0);
	if (a.tv_usec > b.tv_usec)
		return (1);
	return (0);
}

int	check_philosopher(t_philosopher *philo)
{
	t_time			current_time;
	t_time			diff;
	t_time			last_eat_time;
	t_simulation	*simulation;

	simulation = philo->simulation;
	if (get_philo_state(philo) == EATING)
		return (GOOD_PHILO_EXIT);
	if (get_sync_data(&philo->last_eat_time_cs, &last_eat_time, sizeof(t_time)))
		return (BAD_PHILO_EXIT);
	if (gettimeofday(&current_time, NULL))
		return (BAD_PHILO_EXIT);
	diff = time_diff(last_eat_time, current_time);
	if (is_bigger_than(diff, simulation->time_to_die))
	{
		set_sim_state(simulation, TERMINATED);
		simulation->dead_philo_id = philo->id;
		return (BAD_PHILO_EXIT);
	}
	return (GOOD_PHILO_EXIT);
}

int	check_eat_count(t_philosopher *philo, int *full_counter)
{
	int	eat_count;

	if (philo->simulation->must_eat_count <= 0)
		return (GOOD_PHILO_EXIT);
	if (get_sync_data(&philo->eat_count_cs, &eat_count, sizeof(int)))
		return (BAD_PHILO_EXIT);
	if (eat_count >= philo->simulation->must_eat_count)
		*full_counter += 1;
	return (GOOD_PHILO_EXIT);
}

void	*watchdog_routine(void *arg)
{
	t_simulation	*simulation;
	int				full_counter;
	int				i;

	simulation = (t_simulation *) arg;
	while (get_sim_state(simulation) != TERMINATED)
	{
		i = 0;
		full_counter = 0;
		while (i < (int)simulation->philo_count)
		{
			if (check_philosopher(&simulation->philos[i]))
				return (set_sim_state(simulation, TERMINATED), NULL);
			if (check_eat_count(&simulation->philos[i], &full_counter))
				return (set_sim_state(simulation, TERMINATED), NULL);
			i++;
		}
		if (simulation->must_eat_count > 0
			&& full_counter >= (int)simulation->philo_count)
			return (set_sim_state(simulation, TERMINATED), NULL);
		usleep(100);
	}
	return (NULL);
}
