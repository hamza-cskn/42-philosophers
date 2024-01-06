#include <pthread.h>
#include <stdio.h>
#include "philo.h"

int start_simulation(t_simulation *simulation) {
	unsigned int	i;
	simulation->start_time = get_cur_time();
	i = 0;
	while (i < simulation->philo_count)
	{
		if (pthread_create(&simulation->philos[i].thread, NULL, philosopher_routine, &simulation->philos[i]))
			return (BAD_PHILO_EXIT);
		i++;
	}
	if (pthread_create(simulation->watchdog_thread, NULL, watchdog_routine, simulation))
		return (BAD_PHILO_EXIT);
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

int main(int ac, char **av)
{
	t_simulation	simulation;
	pthread_t		watchdog_thread;

	print_mutex = create_lock(); //todo remove it.
	simulation = (t_simulation){0};
	if (create_cs(&simulation.state_cs, sizeof(t_sim_state)))
		return (BAD_PHILO_EXIT);
	*((t_sim_state *)simulation.state_cs.data) = SUSPENDED;
	simulation.philo_count = unsigned_atoi(av[1]);
	simulation.time_to_die = to_timeval(unsigned_atoi(av[2]));
	simulation.time_to_eat = to_timeval(unsigned_atoi(av[3]));
	simulation.time_to_sleep = to_timeval(unsigned_atoi(av[4]));
	simulation.must_eat_count = -1;
	if (ac >= 6)
		simulation.must_eat_count = (int) unsigned_atoi(av[5]);
	simulation.watchdog_thread = &watchdog_thread;
	simulation.dead_philo_id = -1;
	simulation.philos = create_philosophers(&simulation);
	if (!simulation.philos)
		return (BAD_PHILO_EXIT);
	int exit_code = start_simulation(&simulation);
	if (simulation.dead_philo_id != -1)
		SYNC_PRINT("Philosopher %d is dead\n", simulation.dead_philo_id);
	if (exit_code)
		SYNC_PRINT("Error code: %d\n", exit_code);
	return exit_code;
}
