/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wabdella <wabdella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 01:21:57 by wabdella          #+#    #+#             */
/*   Updated: 2026/09/13 01:21:58 by wabdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	log_state(t_coder *coder, char *state)
{
	t_sim	*sim;

	sim = coder->sim;
	pthread_mutex_lock(&sim->print_mutex);
	if (!simulation_stopped(sim))
		printf("%ld %ld %s\n",
			get_elapsed_ms(sim), coder->id, state);
	pthread_mutex_unlock(&sim->print_mutex);
}

void	log_start_compile(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	pthread_mutex_lock(&sim->print_mutex);
	if (!simulation_stopped(sim))
	{
		printf("%ld %ld has taken a dongle\n",
			get_elapsed_ms(sim), coder->id);
		printf("%ld %ld has taken a dongle\n",
			get_elapsed_ms(sim), coder->id);
		pthread_mutex_lock(&coder->state_mutex);
		coder->last_compile_start = get_time_ms();
		pthread_mutex_unlock(&coder->state_mutex);
		printf("%ld %ld is compiling\n",
			get_elapsed_ms(sim), coder->id);
	}
	pthread_mutex_unlock(&sim->print_mutex);
}

void	log_burnout(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	pthread_mutex_lock(&sim->print_mutex);
	printf("%ld %ld burned out\n",
		get_elapsed_ms(sim), coder->id);
	pthread_mutex_unlock(&sim->print_mutex);
}
