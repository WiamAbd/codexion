#include "../codexion.h"

int	start_coders(t_sim *sim)
{
	long	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL,
				coder_routine, &sim->coders[i]) != 0)
		{
			stop_simulation(sim);
			while (--i >= 0)
				pthread_join(sim->coders[i].thread, NULL);
			return (0);
		}
		i++;
	}
	return (1);
}

void	join_coders(t_sim *sim)
{
	long	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
}
