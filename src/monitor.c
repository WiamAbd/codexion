#include "../codexion.h"

static int	all_finished(t_sim *sim)
{
	long	i;
	long	count;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_mutex_lock(&sim->coders[i].state_mutex);
		count = sim->coders[i].compile_count;
		pthread_mutex_unlock(&sim->coders[i].state_mutex);
		if (count < sim->config.number_of_compiles_required)
			return (0);
		i++;
	}
	return (1);
}

static int	coder_burned_out(t_coder *coder)
{
	long	last_compile;
	long	deadline;

	pthread_mutex_lock(&coder->state_mutex);
	last_compile = coder->last_compile_start;
	pthread_mutex_unlock(&coder->state_mutex);
	deadline = last_compile + coder->sim->config.time_to_burnout;
	return (get_time_ms() >= deadline);
}

static t_coder	*find_burnout(t_sim *sim)
{
	long	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		if (coder_burned_out(&sim->coders[i]))
			return (&sim->coders[i]);
		i++;
	}
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	t_sim		*sim;
	t_coder		*burned;

	sim = (t_sim *)arg;
	while (!simulation_stopped(sim))
	{
		burned = find_burnout(sim);
		if (burned)
		{
			log_burnout(burned);
			stop_simulation(sim);
			break ;
		}
		if (all_finished(sim))
		{
			stop_simulation(sim);
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}
