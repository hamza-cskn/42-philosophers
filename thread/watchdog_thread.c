#include "../philo.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/fcntl.h>

/**
 * Compares given timeval to each other
 *
 * @param a first timeval
 * @param b second timeval
 * @return 1 if a is bigger than b, 0 otherwise
 */
int is_bigger_than(t_time a, t_time b)
{
	if (a.tv_sec > b.tv_sec)
		return 1;
	if (a.tv_sec < b.tv_sec)
		return 0;
	if (a.tv_usec > b.tv_usec)
		return 1;
	return 0;
}

void *watchdog_routine(void *arg)
{
	t_simulation	*simulation;
	t_time			current_time;
	t_time			diff;
	t_time			last_eat_time;
	int 			i;
	t_philosopher	*philo;

	simulation = (t_simulation *) arg;
	while (get_sim_state(simulation) != TERMINATED)
	{
		i = 0;
		while (i < (int) simulation->philo_count)
		{
			philo = &simulation->philos[i];
			if (get_philo_state(philo) == EATING)
			{
				i++;
				continue;
			}
			if (get_sync_data(&philo->last_eat_time_cs, &last_eat_time, sizeof(t_time)))
				return NULL;
			if (gettimeofday(&current_time, NULL))
				return set_sim_state(simulation, TERMINATED), NULL;
			diff = time_diff(last_eat_time, current_time);
			if (is_bigger_than(diff, simulation->time_to_die))
			{
				set_sim_state(simulation, TERMINATED);
				simulation->dead_philo_id = i;
				return NULL;
			}
			i++;
		}
		usleep(100);
	}
	return NULL;
}
