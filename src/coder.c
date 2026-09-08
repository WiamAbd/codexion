#include "../codexion.h"

static void	get_dongles(t_coder *coder,
	t_dongle **first, t_dongle **second)
{
	long	index;
	long	next;

	index = coder->id - 1;
	next = (index + 1) % coder->sim->config.number_of_coders;
	if (coder->id % 2 != 0)
	{
		*first = &coder->sim->dongles[index];
		*second = &coder->sim->dongles[next];
	}
	else
	{
		*first = &coder->sim->dongles[next];
		*second = &coder->sim->dongles[index];
	}
}
static int	acquire_two_dongles(t_coder *coder,
	t_dongle **first, t_dongle **second)
{
	get_dongles(coder, first, second);
	if (*first == *second)
		return (0);
	if (!acquire_dongle(coder, *first))
		return (0);
	if (!acquire_dongle(coder, *second))
	{
		release_dongle(coder, *first);
		return (0);
	}
	return (1);
}
static void	release_two_dongles(t_coder *coder,
	t_dongle *first, t_dongle *second)
{
	release_dongle(coder, second);
	release_dongle(coder, first);
}

static void	compile_action(t_coder *coder)
{
	pthread_mutex_lock(&coder->state_mutex);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&coder->state_mutex);
	sim_sleep(coder->sim->config.time_to_compile, coder->sim);
	pthread_mutex_lock(&coder->state_mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->state_mutex);
}

static void	debug_action(t_coder *coder)
{
	sim_sleep(coder->sim->config.time_to_debug, coder->sim);
}

static void	refactor_action(t_coder *coder)
{
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