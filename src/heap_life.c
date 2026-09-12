#include "../codexion.h"

int	init_heap(t_heap *heap, t_sim *sim)
{
	heap->items = malloc(sizeof(t_request) * sim->config.number_of_coders);
	if (!heap->items)
		return (0);
	heap->size = 0;
	heap->sim = sim;
	return (1);
}

void	destroy_heap(t_heap *heap)
{
	free(heap->items);
	heap->items = NULL;
	heap->size = 0;
}
