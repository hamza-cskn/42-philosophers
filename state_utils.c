/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcoskun <hcoskun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 19:09:01 by hcoskun           #+#    #+#             */
/*   Updated: 2024/02/01 14:28:26 by hcoskun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

/**
 * functions in this file makes easy to allow some
 * mutex variables. 
*/

t_sim_state	get_sim_state(t_simulation *sim)
{
	t_sim_state	state;

	if (get_sync_data(&sim->state_cs, &state, sizeof(t_sim_state)))
		return (printf("The simulation terminating.\n"), TERMINATED);
	return (state);
}

int	set_sim_state(t_simulation *sim, t_sim_state state)
{
	return (set_sync_data(&sim->state_cs, &state, sizeof(t_sim_state)));
}

t_philo_state	get_philo_state(t_philosopher *philo)
{
	t_philo_state	state;

	if (get_sync_data(&philo->state_cs, &state, sizeof(t_philo_state)))
		return (UNDEFINED);
	return (state);
}

int	set_philo_state(t_philosopher *philo, t_philo_state state)
{
	return (set_sync_data(&philo->state_cs, &state, sizeof(t_philo_state)));
}
