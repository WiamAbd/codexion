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

void	ms_to_timespec(long ms, struct timespec *time)
{
	time->tv_sec = ms / 1000;
	time->tv_nsec = (ms % 1000) * 1000000;
}

void	sim_sleep(long duration, t_sim *sim)
{
	long	end;

	end = get_time_ms() + duration;
	while (!simulation_stopped(sim)
		&& get_time_ms() < end)
		usleep(500);
}