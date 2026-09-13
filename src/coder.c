/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wabdella <wabdella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 01:21:45 by wabdella          #+#    #+#             */
/*   Updated: 2026/09/13 01:21:46 by wabdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static void	even_odd_get_dongles(t_coder *coder,
	t_dongle **first, t_dongle **second)
{
	long	index;
	long	next;

	index = coder->id - 1;
	next = (index + 1) % coder->sim->config.number_of_coders;
	if (coder->id % 2 != 0)
	{
		*first = &coder->sim->dongles[index];
		*second = &coder->sim->dongles[next];
	}
	else
	{
		*first = &coder->sim->dongles[next];
		*second = &coder->sim->dongles[index];
	}
}

int	acquire_two_dongles(t_coder *coder,
	t_dongle **first, t_dongle **second)
{
	long	arrival_time;

	even_odd_get_dongles(coder, first, second);
	if (*first == *second)
		return (0);
	arrival_time = get_time_ms();
	if (!acquire_dongle(coder, *first, arrival_time))
		return (0);
	if (!acquire_dongle(coder, *second, arrival_time))
	{
		release_dongle(coder, *first);
		return (0);
	}
	return (1);
}

void	release_two_dongles(t_coder *coder,
	t_dongle *first, t_dongle *second)
{
	release_dongle(coder, first);
	release_dongle(coder, second);
}
