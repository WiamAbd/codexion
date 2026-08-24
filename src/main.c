#include "../codexion.h"
#include <stdio.h>

int	main(int argc, char **argv)
{
	t_config	config;

	if (!parse_arguments(argc, argv, &config))
	{
		printf("Error: invalid arguments\n");
		return (1);
	}
	printf("number_of_coders: %ld\n", config.number_of_coders);
	printf("time_to_burnout: %ld\n", config.time_to_burnout);
	printf("time_to_compile: %ld\n", config.time_to_compile);
	printf("time_to_debug: %ld\n", config.time_to_debug);
	printf("time_to_refactor: %ld\n", config.time_to_refactor);
	printf("compiles_required: %ld\n", config.number_of_compiles_required);
	printf("dongle_cooldown: %ld\n", config.dongle_cooldown);
	printf("scheduler: %s\n", config.scheduler);
	return (0);
}