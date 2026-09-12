#include "../codexion.h"

static void	init_start_time(t_sim *sim)
{
	long	i;

	sim->start_time = get_time_ms();
	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_mutex_lock(&sim->coders[i].state_mutex);
		sim->coders[i].last_compile_start = sim->start_time;
		pthread_mutex_unlock(&sim->coders[i].state_mutex);
		i++;
	}
}

int	simulation_stopped(t_sim *sim)
{
	int	stop;

	pthread_mutex_lock(&sim->state_mutex);
	stop = sim->stop;
	pthread_mutex_unlock(&sim->state_mutex);
	return (stop);
}

void	stop_simulation(t_sim *sim)
{
	long	i;

	pthread_mutex_lock(&sim->state_mutex);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->state_mutex);
	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_mutex_lock(&sim->dongles[i].mutex);
		pthread_cond_broadcast(&sim->dongles[i].cond);
		pthread_mutex_unlock(&sim->dongles[i].mutex);
		i++;
	}
}

int	start_simulation(t_sim *sim)
{
	init_start_time(sim);
	if (!start_coders(sim))
		return (0);
	if (pthread_create(&sim->monitor, NULL,
			monitor_routine, sim) != 0)
	{
		stop_simulation(sim);
		join_coders(sim);
		return (0);
	}
	pthread_join(sim->monitor, NULL);
	join_coders(sim);
	return (1);
}
