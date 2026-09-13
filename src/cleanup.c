/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wabdella <wabdella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 01:21:42 by wabdella          #+#    #+#             */
/*   Updated: 2026/09/13 01:21:42 by wabdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	destroy_coders(t_sim *sim, long count)
{
	long	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&sim->coders[i].state_mutex);
		i++;
	}
	free(sim->coders);
	sim->coders = NULL;
}

void	destroy_dongles(t_sim *sim, long count)
{
	long	i;

	i = 0;
	while (i < count)
	{
		destroy_heap(&sim->dongles[i].heap);
		pthread_cond_destroy(&sim->dongles[i].cond);
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		i++;
	}
	free(sim->dongles);
	sim->dongles = NULL;
}

void	cleanup_simulation(t_sim *sim)
{
	destroy_dongles(sim, sim->config.number_of_coders);
	pthread_mutex_destroy(&sim->state_mutex);
	pthread_mutex_destroy(&sim->print_mutex);
	destroy_coders(sim, sim->config.number_of_coders);
}
