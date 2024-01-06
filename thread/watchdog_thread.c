#include "../philo.h"
#include <stdio.h>
#include <unistd.h>

void *watchdog_routine(void *arg)
{
	t_simulation	*simulation;
	t_time			current_time;
	t_time			diff;
	t_time			last_eat_time;
	unsigned int 	i;
	t_sim_state 	state;
	t_philosopher	*philo;

	simulation = (t_simulation *) arg;
	while (get_sim_state(simulation) != TERMINATED)
	{
		i = 0;
		gettimeofday(&current_time, NULL);
		while (i < simulation->philo_count)
		{
			philo = &simulation->philos[i];

			if (get_sync_data(&philo->last_eat_time_cs, &last_eat_time, sizeof(t_time)))
				return NULL;

			if (get_sync_data(&philo->state_cs, &state, sizeof(t_sim_state)))
				return NULL;

			diff = time_diff(last_eat_time, current_time);
			if (state != EATING && diff.tv_sec >= simulation->time_to_die.tv_sec && diff.tv_usec > simulation->time_to_die.tv_usec)
			{
				set_sim_state(simulation, TERMINATED);
				SYNC_PRINT("[%ld.%d] Philosopher %d is starve because he could not eat meal since %ld.%d and for %ld.%d\n", get_cur_time().tv_sec, get_cur_time().tv_usec, i, last_eat_time.tv_sec, last_eat_time.tv_usec, diff.tv_sec, diff.tv_usec);
				simulation->dead_philo_id = i;
				return NULL;
			}
			i++;
		}
		usleep(1000);
	}
	return NULL;
}
