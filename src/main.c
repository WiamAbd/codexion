/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wabdella <wabdella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 01:21:59 by wabdella          #+#    #+#             */
/*   Updated: 2026/09/13 01:21:59 by wabdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	main(int argc, char **argv)
{
	t_sim	sim;

	if (!parse_arguments(argc, argv, &sim.config))
		return (1);
	if (!init_simulation(&sim))
		return (1);
	if (!start_simulation(&sim))
	{
		cleanup_simulation(&sim);
		return (1);
	}
	cleanup_simulation(&sim);
	return (0);
}
