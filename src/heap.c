/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wabdella <wabdella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 01:21:53 by wabdella          #+#    #+#             */
/*   Updated: 2026/09/13 01:21:54 by wabdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		sift_down(heap, 0);
	}
	return (1);
}

int	heap_remove_coder(t_heap *heap, t_coder *coder)
{
	long	i;

	i = 0;
	while (i < heap->size
		&& heap->items[i].coder != coder)
		i++;
	if (i == heap->size)
		return (0);
	heap->size--;
	if (i != heap->size)
	{
		heap->items[i] = heap->items[heap->size];
		heap_repair(heap, i);
	}
	return (1);
}
