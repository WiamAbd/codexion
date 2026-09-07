#include "../codexion.h"
#include <stdio.h>

static void	print_initialization(t_sim *sim)
{
	long	i;

	printf("----- CODERS -----\n");
	i = 0;
	while (i < sim->config.number_of_coders)
	{
		printf("Coder[%ld]: id=%ld, compiles=%ld\n",
			i, sim->coders[i].id,
			sim->coders[i].compile_count);
		i++;
	}
	printf("\n----- DONGLES -----\n");
	i = 0;
	while (i < sim->config.number_of_coders)
	{
		printf("Dongle[%ld]: available=%d, heap_size=%ld\n",
			i, sim->dongles[i].available,
			sim->dongles[i].heap.size);
		i++;
	}
}

static void	fill_test_heap(t_sim *sim)
{
	t_request	request;
	long		i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		request.coder = &sim->coders[i];
		request.arrival_time
			= (sim->config.number_of_coders - i) * 100;
		request.deadline = (i + 1) * 200;
		if (!heap_push(&sim->dongles[0].heap, request))
			printf("Failed to push request\n");
		i++;
	}
}

static void	print_heap_order(t_sim *sim)
{
	t_request	request;

	printf("\n----- HEAP POP ORDER -----\n");
	while (heap_pop(&sim->dongles[0].heap, &request))
	{
		printf("Coder %ld | arrival=%ld | deadline=%ld\n",
			request.coder->id,
			request.arrival_time,
			request.deadline);
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
	print_initialization(&sim);
	fill_test_heap(&sim);
	print_heap_order(&sim);
	cleanup_simulation(&sim);
	return (0);
}