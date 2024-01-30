#include <pthread.h>
#include <stdio.h>
#include "philo.h"
#include <stdlib.h> 

int start_simulation(t_simulation *simulation) {
	unsigned int	i;
	i = 0;
	while (i < simulation->philo_count)
	{
		if (pthread_create(&simulation->philos[i].thread, NULL, philosopher_routine, &simulation->philos[i]))
			return (BAD_PHILO_EXIT);
		i++;
	}
	if (pthread_create(simulation->watchdog_thread, NULL, watchdog_routine, simulation))
		return (BAD_PHILO_EXIT);
	simulation->start_time = get_cur_time();
	set_sim_state(simulation, RUNNING);
	i = 0;
	while (i < simulation->philo_count)
	{
		if (pthread_join(simulation->philos[i].thread, NULL))
			return (abort_simulation(simulation), BAD_PHILO_EXIT);
		i++;
	}
	if (pthread_join(*simulation->watchdog_thread, NULL))
		return (abort_simulation(simulation), BAD_PHILO_EXIT);
	return (GOOD_PHILO_EXIT);
}

int deserialize_time(t_time *time, char *str)
{
	long long ms = unsigned_atoi(str);
	if (ms < 0)
	{
		printf("%s is not a valid number for ms.\n", str);
		return (BAD_PHILO_EXIT);
	}
	*time = (t_time){ms / 1000ll, (int) ((ms % 1000ll) * 1000ll)};
	return (GOOD_PHILO_EXIT);
}

int init_simulation(t_simulation *sim, pthread_t *watchdog_thread, int ac, char **av)
{
	*sim = (t_simulation){0};
	if (create_cs(&sim->state_cs, sizeof(t_sim_state)))
		return (BAD_PHILO_EXIT);
	*((t_sim_state *)sim->state_cs.data) = SUSPENDED;
	sim->philo_count = unsigned_atoi(av[1]);
	if (sim->philo_count < 1)
		return (BAD_PHILO_EXIT);
	if (deserialize_time(&sim->time_to_die, av[2]))
		return (BAD_PHILO_EXIT);
	if (deserialize_time(&sim->time_to_eat, av[3]))
		return (BAD_PHILO_EXIT);
	if (deserialize_time(&sim->time_to_sleep, av[4]))
		return (BAD_PHILO_EXIT);
	sim->sticks = create_sticks((int) sim->philo_count);
	if (!sim->sticks)
		return BAD_PHILO_EXIT;
	sim->must_eat_count = -1;
	if (ac >= 6)
		sim->must_eat_count = (int) unsigned_atoi(av[5]);
	sim->watchdog_thread = watchdog_thread;
	sim->dead_philo_id = -1;
	sim->philos = create_philosophers(sim);
	if (!sim->philos)
		return (BAD_PHILO_EXIT);
	return (GOOD_PHILO_EXIT);
}

int main(int ac, char **av)
{
	t_simulation	simulation;
	pthread_t		watchdog_thread;

	print_mutex = create_lock(); //todo remove it.
	if (ac != 5 && ac != 6)
	{
		printf("Usage: %s <philo count> <time> <time> <time>\n", av[0]);
		return 1;
	}
	if (init_simulation(&simulation, &watchdog_thread, ac, av))
		return (abort_simulation(&simulation), BAD_PHILO_EXIT);
	int exit_code = start_simulation(&simulation);
	if (simulation.dead_philo_id != -1)
		SYNC_PRINT("Philosopher %d is dead\n", simulation.dead_philo_id);
	if (exit_code)
		SYNC_PRINT("Error code: %d\n", exit_code);

	return exit_code;
}
