#include "../codexion.h"
#include <stdio.h>

static void	print_test(t_sim *sim)
{
	long	i;

	printf("----- CONFIG -----\n");
	printf("Coders: %ld\n", sim->config.number_of_coders);
	printf("Burnout: %ld\n", sim->config.time_to_burnout);
	printf("Scheduler: %s\n", sim->config.scheduler);
	printf("\n----- CODERS -----\n");
	i = 0;
	while (i < sim->config.number_of_coders)
	{
		printf("Coder[%ld]: id=%ld, compiles=%ld\n",
			i, sim->coders[i].id, sim->coders[i].compile_count);
		i++;
	}
	printf("\n----- DONGLES -----\n");
	i = 0;
	while (i < sim->config.number_of_coders)
	{
		printf("Dongle[%ld]: available=%d, available_at=%ld\n",
			i, sim->dongles[i].available,
			sim->dongles[i].available_at);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_sim	sim;

	if (!parse_arguments(argc, argv, &sim.config))
	{
		printf("Error: invalid arguments\n");
		return (1);
	}
	if (!init_simulation(&sim))
	{
		printf("Error: initialization failed\n");
		return (1);
	}
	print_test(&sim);
	cleanup_simulation(&sim);
	return (0);
}