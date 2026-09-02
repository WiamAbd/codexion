#include "../codexion.h"

static void	swap(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static int	compare(t_heap *heap,
	t_request *a, t_request *b)
{
	if (strcmp(heap->sim->config.scheduler, "fifo") == 0)
		return (a->arrival_time < b->arrival_time);
	return (a->deadline < b->deadline);
}

static void	sift_up(t_heap *heap, long index)
{
	long	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (!compare(heap, &heap->items[index],
				&heap->items[parent]))
			break ;
		swap(&heap->items[index], &heap->items[parent]);
		index = parent;
	}
}
static void	sift_down(t_heap *heap)
{
	long	left;
	long	right;
	long	best;
    long    index;
    index = 0;

	while (1)
	{
		left = index * 2 + 1;
		right = index * 2 + 2;
		best = index;
		if (left < heap->size
			&& compare(heap, &heap->items[left],
				&heap->items[best]))
			best = left;
		if (right < heap->size
			&& compare(heap, &heap->items[right],
				&heap->items[best]))
			best = right;
		if (best == index)
			break ;
		swap(&heap->items[index], &heap->items[best]);
		index = best;
	}
}

int	heap_push(t_heap *heap, t_request request)
{
	long	index;

	if (heap->size >= heap->capacity)
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

int	init_heap(t_heap *heap, long capacity, t_sim *sim)
{
	heap->items = malloc(sizeof(t_request) * capacity);
	if (!heap->items)
		return (0);
	heap->size = 0;
	heap->capacity = capacity;
	heap->sim = sim;
	return (1);
}
void	destroy_heap(t_heap *heap)
{
	free(heap->items);
	heap->items = NULL;
	heap->size = 0;
	heap->capacity = 0;
}