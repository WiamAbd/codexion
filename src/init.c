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
		i++;
	}
	return (1);
}

static void	destroy_dongles(t_sim *sim, long count)
{
	long	i;

	i = 0;
	while (i < count)
	{
		pthread_cond_destroy(&sim->dongles[i].cond);
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		i++;
	}
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
		sim->dongles[i].available = 1;
		sim->dongles[i].available_at = 0;
		if (pthread_mutex_init(&sim->dongles[i].mutex, NULL) != 0)
			return (destroy_dongles(sim, i), free(sim->dongles), 0);
		if (pthread_cond_init(&sim->dongles[i].cond, NULL) != 0)
		{
			pthread_mutex_destroy(&sim->dongles[i].mutex);
			destroy_dongles(sim, i);
			free(sim->dongles);
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
		free(sim->coders);
		return (0);
	}
	if (!init_dongles(sim))
	{
		pthread_mutex_destroy(&sim->state_mutex);
		pthread_mutex_destroy(&sim->print_mutex);
		free(sim->coders);
		return (0);
	}
	return (1);
}