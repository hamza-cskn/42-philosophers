#include "philo.h"

t_sim_state get_sim_state(t_simulation *sim) {
	t_sim_state state;
	if (get_sync_data(&sim->state_cs, &state, sizeof(t_sim_state)))
		return prnt_err("The simulation terminating."), TERMINATED;
	return state;
}

int set_sim_state(t_simulation *sim, t_sim_state state) {
	return set_sync_data(&sim->state_cs, &state, sizeof(t_sim_state));
}

t_philo_state get_philo_state(t_philosopher *philo) {
	t_philo_state state;
	if (get_sync_data(&philo->state_cs, &state, sizeof(t_philo_state)))
		return UNDEFINED;
	return state;
}

int set_philo_state(t_philosopher *philo, t_philo_state state) {
	return set_sync_data(&philo->state_cs, &state, sizeof(t_philo_state));
}


