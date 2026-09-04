#include "../codexion.h"

static void	swap(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static int	compare(t_heap *heap, t_request *a, t_request *b)
{
	if (strcmp(heap->sim->config.scheduler, "fifo") == 0
		&& a->arrival_time != b->arrival_time)
		return (a->arrival_time < b->arrival_time);
	else if (strcmp(heap->sim->config.scheduler, "edf") == 0
		&& a->deadline != b->deadline)
		return (a->deadline < b->deadline);
	return (a->coder->id < b->coder->id);
}


void	sift_up(t_heap *heap, long index)
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
void	sift_down(t_heap *heap)
{
	long	left;
	long	right;
	long	best;
	long	index;

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