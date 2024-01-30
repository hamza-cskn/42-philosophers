#include "../philo.h"
#include <stdio.h>
#include <unistd.h>

int update_last_eat_time(t_philosopher *philo) {
	t_time last_eat_time;
	if (gettimeofday(&last_eat_time, NULL))
		return prnt_err("update_last_eat_time: time could not get"), BAD_PHILO_EXIT;
	if (set_sync_data(&philo->last_eat_time_cs, &last_eat_time, sizeof(t_time)))
		return BAD_PHILO_EXIT;
	return GOOD_PHILO_EXIT;
}

int	eat(t_philosopher *philo)
{
	pthread_mutex_t *left_stick;
	pthread_mutex_t *right_stick;
	
	left_stick = philo->simulation->sticks[philo->id];
	right_stick = philo->simulation->sticks[(philo->id + 1) % philo->simulation->philo_count];

	if (left_stick == right_stick)
		return (GOOD_PHILO_EXIT);
	if (pthread_mutex_lock(left_stick))
		return (prnt_err("eat: left stick could not locked"), BAD_PHILO_EXIT);
	if (pthread_mutex_lock(right_stick))
		return (pthread_mutex_unlock(left_stick), prnt_err("eat: left stick could not locked"), BAD_PHILO_EXIT);
	if (set_philo_state(philo, EATING))
		return (BAD_PHILO_EXIT);
	if (get_sim_state(philo->simulation) == RUNNING)
	{
		update_last_eat_time(philo);
		SYNC_PRINT("%llu %d is eating\n", get_timestamp(philo->simulation->start_time), philo->id);
		suspend_thread(philo->simulation->time_to_eat);
	}
	if (pthread_mutex_unlock(left_stick) || pthread_mutex_unlock(right_stick))
		return (prnt_err("eat: all sticks could not unlocked"), BAD_PHILO_EXIT);
	if (set_philo_state(philo, SLEEPING))
		return (BAD_PHILO_EXIT);
	return (GOOD_PHILO_EXIT);
}

void *philosopher_routine(void *arg)
{
	t_philosopher *philo;
	t_philo_state state;

	philo = (t_philosopher *)arg;
	while (get_sim_state(philo->simulation) == SUSPENDED)
		;
	if (get_sim_state(philo->simulation) == TERMINATED)
		return NULL;
	if (philo->id % 2 == 0)
		usleep(3000);
	update_last_eat_time(philo);
	while (get_sim_state(philo->simulation) == RUNNING)
	{
		if (get_sync_data(&philo->state_cs, &state, sizeof(t_philo_state)))
			return set_sim_state(philo->simulation, TERMINATED), NULL;
		if (state == SLEEPING)
		{
			SYNC_PRINT("%llu %d is sleeping\n", get_timestamp(philo->simulation->start_time), philo->id);
			suspend_thread(philo->simulation->time_to_sleep);
			if (get_sim_state(philo->simulation) == TERMINATED)
				return NULL;
			SYNC_PRINT("%llu %d is thinking\n", get_timestamp(philo->simulation->start_time), philo->id);
			if (set_philo_state(philo, THINKING))
				return set_sim_state(philo->simulation, TERMINATED), NULL;
		}
		else if (state == THINKING && eat(philo))
			return set_sim_state(philo->simulation, TERMINATED), NULL;
	}
	return NULL;
}

