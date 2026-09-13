/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wabdella <wabdella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 01:21:35 by wabdella          #+#    #+#             */
/*   Updated: 2026/09/13 01:21:36 by wabdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static void	compile_action(t_coder *coder)
{
	log_start_compile(coder);
	sim_sleep(coder->sim->config.time_to_compile, coder->sim);
	pthread_mutex_lock(&coder->state_mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->state_mutex);
}

static void	debug_action(t_coder *coder)
{
	log_state(coder, "is debugging");
	sim_sleep(coder->sim->config.time_to_debug, coder->sim);
}

static void	refactor_action(t_coder *coder)
{
	log_state(coder, "is refactoring");
	sim_sleep(coder->sim->config.time_to_refactor, coder->sim);
}

void	*coder_routine(void *arg)
{
	t_coder		*coder;
	t_dongle	*first;
	t_dongle	*second;

	coder = (t_coder *)arg;
	while (!simulation_stopped(coder->sim))
	{
		if (!acquire_two_dongles(coder, &first, &second))
			break ;
		compile_action(coder);
		release_two_dongles(coder, first, second);
		if (simulation_stopped(coder->sim))
			break ;
		debug_action(coder);
		if (simulation_stopped(coder->sim))
			break ;
		refactor_action(coder);
	}
	return (NULL);
}
