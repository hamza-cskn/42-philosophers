#ifndef PHILO_H
#define PHILO_H

#define BAD_PHILO_EXIT 1
#define GOOD_PHILO_EXIT 0

#define SYNC_PRINT(...) pthread_mutex_lock(print_mutex), printf(__VA_ARGS__), pthread_mutex_unlock(print_mutex)

#include <pthread.h>
#include <sys/time.h>

typedef struct timeval t_time;

typedef enum e_philo_state
{
	UNDEFINED,
	EATING,
	THINKING,
	SLEEPING,
} t_philo_state;

typedef enum e_sim_state
{
	RUNNING,
	SUSPENDED,
	TERMINATED,
} t_sim_state;

typedef struct s_critical_section
{
	pthread_mutex_t *mutex;
	void *data;
} t_critical_section;

typedef struct s_simulation t_simulation;
pthread_mutex_t *print_mutex;

typedef struct s_philosopher {
	pthread_t thread;
	t_critical_section last_eat_time_cs;
	t_critical_section state_cs;
	unsigned int id;
	t_simulation *simulation;
} t_philosopher;

typedef struct s_simulation {
	unsigned int philo_count;
	t_philosopher *philos;
	pthread_mutex_t **sticks;
	t_critical_section state_cs;
	int dead_philo_id;
	pthread_t *watchdog_thread;
	t_time time_to_die;
	t_time time_to_eat;
	t_time time_to_sleep;
	t_time start_time;
	int must_eat_count;
} t_simulation;

long long unsigned_atoi(const char *str);
pthread_mutex_t *create_lock();
pthread_mutex_t **create_sticks(int amount);
t_time get_cur_time();
t_time time_diff(t_time start, t_time end);
t_time to_timeval(long long ms);
void *ft_calloc(unsigned long size, int count);
void abort_mutex(pthread_mutex_t **mutex);
void abort_sticks(pthread_mutex_t **sticks, int count);
void abort_simulation(t_simulation *simulation);
void safe_free(void *ptr);
void prnt_err(char *msg);

t_philosopher	*create_philosophers(t_simulation *simulation);
void	free_philosophers(t_philosopher *philos, int count);

int get_sync_data(t_critical_section *cs, void *addr, unsigned int size);
int set_sync_data(t_critical_section *cs, void *data, unsigned int size);
int create_cs(t_critical_section *addr, unsigned int size);
void abort_critical_section(t_critical_section cs);

void *watchdog_routine(void *arg);
void *philosopher_routine(void *arg);

t_sim_state get_sim_state(t_simulation *sim);
int set_sim_state(t_simulation *sim, t_sim_state state);

t_philo_state get_philo_state(t_philosopher *philo);
int set_philo_state(t_philosopher *philo, t_philo_state state);

#endif