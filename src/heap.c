#include "../codexion.h"



int	heap_push(t_heap *heap, t_request request)
{
	long	index;

	if (heap->size >= heap->sim->config.number_of_coders)
		return (0);
	index = heap->size;
	heap->items[index] = request;
	heap->size++;
	sift_up(heap, index);
	return (1);
}
t_request	*heap_peek(t_heap *heap)
{
	if (heap->size == 0)
		return (NULL);
	return (&heap->items[0]);
}


int	heap_pop(t_heap *heap, t_request *result)
{
	if (heap->size == 0)
		return (0);
	*result = heap->items[0];
	heap->size--;
	if (heap->size > 0)
	{
		heap->items[0] = heap->items[heap->size];
		sift_down(heap);
	}
	return (1);
}
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