/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_thread.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcoskun <hcoskun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 19:16:27 by hcoskun           #+#    #+#             */
/*   Updated: 2024/01/30 19:30:58 by hcoskun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"
#include <stdio.h>
#include <unistd.h>

int	increase_eat_count(t_philosopher *philo)
{
	int	count;

	if (philo->simulation->must_eat_count < 0)
		return (GOOD_PHILO_EXIT);
	if (get_sync_data(&philo->eat_count_cs, &count, sizeof(int)))
		return (BAD_PHILO_EXIT);
	count++;
	if (set_sync_data(&philo->eat_count_cs, &count, sizeof(int)))
		return (BAD_PHILO_EXIT);
	return (GOOD_PHILO_EXIT);
}

int	update_last_eat_time(t_philosopher *philo)
{
	t_time	last_eat_time;

	if (gettimeofday(&last_eat_time, NULL))
		return (printf("update_last_eat_time: error\n"), BAD_PHILO_EXIT);
	if (set_sync_data(&philo->last_eat_time_cs, &last_eat_time, sizeof(t_time)))
		return (BAD_PHILO_EXIT);
	return (GOOD_PHILO_EXIT);
}

int	eat(t_philosopher *p)
{
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;

	left = p->simulation->sticks[p->id];
	right = p->simulation->sticks[(p->id + 1) % p->simulation->philo_count];
	if (left == right)
		return (GOOD_PHILO_EXIT);
	if (pthread_mutex_lock(left))
		return (printf("eat: left stick could not locked\n"), BAD_PHILO_EXIT);
	if (pthread_mutex_lock(right))
		return (pthread_mutex_unlock(left), BAD_PHILO_EXIT);
	if (set_philo_state(p, EATING))
		return (BAD_PHILO_EXIT);
	if (get_sim_state(p->simulation) == RUNNING)
	{
		if (update_last_eat_time(p) || increase_eat_count(p))
			return (BAD_PHILO_EXIT);
		sync_print("%llu %d is eating\n", p);
		suspend_thread(p->simulation->time_to_eat);
	}
	if (pthread_mutex_unlock(left) || pthread_mutex_unlock(right))
		return (printf("eat: all sticks could not unlocked\n"), BAD_PHILO_EXIT);
	if (set_philo_state(p, SLEEPING))
		return (BAD_PHILO_EXIT);
	return (GOOD_PHILO_EXIT);
}

int	rest(t_philosopher *philo)
{
	sync_print("%llu %d is sleeping\n", philo);
	suspend_thread(philo->simulation->time_to_sleep);
	if (get_sim_state(philo->simulation) == TERMINATED)
		return (BAD_PHILO_EXIT);
	sync_print("%llu %d is thinking\n", philo);
	if (set_philo_state(philo, THINKING))
		return (BAD_PHILO_EXIT);
	return (GOOD_PHILO_EXIT);
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;
	t_philo_state	state;

	philo = (t_philosopher *)arg;
	while (get_sim_state(philo->simulation) == SUSPENDED)
		;
	if (get_sim_state(philo->simulation) == TERMINATED)
		return (NULL);
	if (philo->id % 2 == 0)
		usleep(3000);
	update_last_eat_time(philo);
	while (get_sim_state(philo->simulation) == RUNNING)
	{
		if (get_sync_data(&philo->state_cs, &state, sizeof(t_philo_state)))
			return (set_sim_state(philo->simulation, TERMINATED), NULL);
		if (state == SLEEPING && rest(philo))
			return (set_sim_state(philo->simulation, TERMINATED), NULL);
		else if (state == THINKING && eat(philo))
			return (set_sim_state(philo->simulation, TERMINATED), NULL);
	}
	return (NULL);
}
