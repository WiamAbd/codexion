#include "../codexion.h"

static void	cleanup_dongles(t_sim *sim)
{
	long	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_cond_destroy(&sim->dongles[i].cond);
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		i++;
	}
	free(sim->dongles);
}

void	cleanup_simulation(t_sim *sim)
{
	cleanup_dongles(sim);
	pthread_mutex_destroy(&sim->state_mutex);
	pthread_mutex_destroy(&sim->print_mutex);
	free(sim->coders);
}