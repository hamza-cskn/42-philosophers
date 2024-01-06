#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "philo.h"

/**
 * This function is accepts null pointers and frees them
 * if they are not null. Before every free operation, the
 * pointer is set to zero and the memory is zeroed out.
 *
 * @param ptr
 */
void safe_free(void *ptr) {
	if (ptr)
		free(ptr);
}

void abort_mutex(pthread_mutex_t **mutex) {
	if (!mutex || !*mutex)
		return;
	pthread_mutex_destroy(*mutex);
	free(*mutex);
	*mutex = NULL;
}

void abort_sticks(pthread_mutex_t **sticks, int count) {
	int i;

	i = 0;
	while (i < count)
		abort_mutex(&sticks[i++]);
}

void abort_simulation(t_simulation *simulation) {
	unsigned int i;

	i = 0;
	while (i < simulation->philo_count)
	{
		if (simulation->philos[i].thread)
			pthread_detach(simulation->philos[i].thread);
	}
	abort_sticks(simulation->sticks, simulation->philo_count);
	safe_free(simulation->philos);
}

void prnt_err(char *msg) {
	write(2, msg, strlen(msg));
	write(2, "\n", 1);
}