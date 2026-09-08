#include "../codexion.h"
void	log_state(t_coder *coder, char *state)
{
	t_sim	*sim;

	sim = coder->sim;
	pthread_mutex_lock(&sim->print_mutex);
	if (!simulation_stopped(sim))
		printf("%ld %ld %s\n",
			get_elapsed_ms(sim), coder->id, state);
	pthread_mutex_unlock(&sim->print_mutex);
}

void	log_burnout(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	pthread_mutex_lock(&sim->print_mutex);
	printf("%ld %ld burned out\n",
		get_elapsed_ms(sim), coder->id);
	pthread_mutex_unlock(&sim->print_mutex);
}