#include "../codexion.h"

long	get_time_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

long	get_elapsed_ms(t_sim *sim)
{
	return (get_time_ms() - sim->start_time);
}