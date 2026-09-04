#include "../codexion.h"

void	destroy_dongles(t_sim *sim, long count)
{
	long	i;

	i = 0;
	while (i < count)
	{
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
	free(sim->coders);
    sim->coders = NULL;
}