#include "../codexion.h"

static int	init_coders(t_sim *sim)
{
	long	i;

	sim->coders = malloc(sizeof(t_coder)
			* sim->config.number_of_coders);
	if (!sim->coders)
		return (0);
	i = 0;
	while (i < sim->config.number_of_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].last_compile_start = 0;
		sim->coders[i].compile_count = 0;
		sim->coders[i].sim = sim;
		if (pthread_mutex_init(&sim->coders[i].state_mutex, NULL) != 0)
		{
			destroy_coders(sim, i);
			return (0);
		}
		i++;
	}
	return (1);
}

static int	init_one_dongle(t_sim *sim, long i)
{
	t_dongle	*dongle;

	dongle = &sim->dongles[i];
	dongle->available = 1;
	dongle->available_at = 0;
	if (pthread_mutex_init(&dongle->mutex, NULL) != 0)
		return (0);
	if (pthread_cond_init(&dongle->cond, NULL) != 0)
	{
		pthread_mutex_destroy(&dongle->mutex);
		return (0);
	}
	if (!init_heap(&dongle->heap, sim))
	{
		pthread_cond_destroy(&dongle->cond);
		pthread_mutex_destroy(&dongle->mutex);
		return (0);
	}
	return (1);
}

static int	init_dongles(t_sim *sim)
{
	long	i;

	sim->dongles = malloc(sizeof(t_dongle)
			* sim->config.number_of_coders);
	if (!sim->dongles)
		return (0);
	i = 0;
	while (i < sim->config.number_of_coders)
	{
		if (!init_one_dongle(sim, i))
		{
			destroy_dongles(sim, i);
			return (0);
		}
		i++;
	}
	return (1);
}

static int	init_mutexes(t_sim *sim)
{
	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&sim->state_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->print_mutex);
		return (0);
	}
	return (1);
}

int	init_simulation(t_sim *sim)
{
	sim->coders = NULL;
	sim->dongles = NULL;
	sim->stop = 0;
	sim->start_time = 0;
	if (!init_coders(sim))
		return (0);
	if (!init_mutexes(sim))
	{
		destroy_coders(sim, sim->config.number_of_coders);
		return (0);
	}
	if (!init_dongles(sim))
	{
		pthread_mutex_destroy(&sim->state_mutex);
		pthread_mutex_destroy(&sim->print_mutex);
		destroy_coders(sim, sim->config.number_of_coders);
		return (0);
	}
	return (1);
}
