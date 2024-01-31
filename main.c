/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcoskun <hcoskun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 18:59:55 by hcoskun           #+#    #+#             */
/*   Updated: 2024/01/30 20:19:13 by hcoskun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "philo.h"

int	start_simulation(t_simulation *simulation)
{
	unsigned int	i;

	i = 0;
	while (i < simulation->philo_count)
	{
		if (pthread_create(&simulation->philos[i].thread, NULL,
				philosopher_routine, &simulation->philos[i]))
			return (BAD_PHILO_EXIT);
		i++;
	}
	if (pthread_create(simulation->watchdog_thread, NULL,
			watchdog_routine, simulation))
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

int	deserialize_time(t_time *time, char *str)
{
	long long	ms;

	ms = unsigned_atoi(str);
	if (ms < 0)
	{
		printf("%s is not a valid number for ms.\n", str);
		return (BAD_PHILO_EXIT);
	}
	*time = (t_time){ms / 1000ll, (int)((ms % 1000ll) * 1000ll)};
	return (GOOD_PHILO_EXIT);
}

int	init_simulation(t_simulation *sim, pthread_t *watchdog, int ac, char **av)
{
	if (create_cs(&sim->state_cs, sizeof(t_sim_state)))
		return (BAD_PHILO_EXIT);
	if (pthread_mutex_init(&sim->print_mutex, NULL))
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
		return (BAD_PHILO_EXIT);
	if (ac >= 6)
		sim->must_eat_count = (int) unsigned_atoi(av[5]);
	sim->watchdog_thread = watchdog;
	sim->dead_philo_id = -1;
	sim->philos = create_philosophers(sim);
	if (!sim->philos)
		return (BAD_PHILO_EXIT);
	return (GOOD_PHILO_EXIT);
}

void	sync_print(char *msg, t_philosopher *philo)
{
	pthread_mutex_lock(&philo->simulation->print_mutex);
	printf(msg, get_timestamp(philo->simulation->start_time), philo->id + 1);
	pthread_mutex_unlock(&philo->simulation->print_mutex);
}

int	main(int ac, char **av)
{
	t_simulation	sim;
	pthread_t		watchdog_thread;
	int				exit_code;

	if (ac != 5 && ac != 6)
	{
		printf("Usage: %s <philo count> <time> <time> <time>\n", av[0]);
		return (1);
	}
	sim = (t_simulation){0};
	sim.must_eat_count = -1;
	if (init_simulation(&sim, &watchdog_thread, ac, av))
		return (abort_simulation(&sim), BAD_PHILO_EXIT);
	exit_code = start_simulation(&sim);
	if (sim.dead_philo_id != -1)
		sync_print("%llu %d died\n", &sim.philos[sim.dead_philo_id]);
	if (exit_code)
		printf("Error code: %d\n", exit_code);
	abort_simulation(&sim);
	return (exit_code);
}
