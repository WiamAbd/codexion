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