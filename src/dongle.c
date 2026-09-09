
#include "../codexion.h"


static t_request	create_request(t_coder *coder, long arrival_time)
{
	t_request	request;
	long		last_compile;

	pthread_mutex_lock(&coder->state_mutex);
	last_compile = coder->last_compile_start;
	pthread_mutex_unlock(&coder->state_mutex);
	request.coder = coder;
	request.arrival_time = arrival_time;
	request.deadline = last_compile
		+ coder->sim->config.time_to_burnout;
	return (request);
}

static int	can_take(t_coder *coder, t_dongle *dongle)
{
	t_request	*request;

	request = heap_peek(&dongle->heap);
	if (!request)
		return (0);
	if (!dongle->available)
		return (0);
	if (get_time_ms() < dongle->available_at)
		return (0);
	if (request->coder != coder)
		return (0);
	return (1);
}

static void	wait_dongle(t_dongle *dongle)
{
	struct timespec	timeout;

	if (dongle->available
		&& get_time_ms() < dongle->available_at)
	{
		ms_to_timespec(dongle->available_at, &timeout);
		pthread_cond_timedwait(&dongle->cond,
			&dongle->mutex, &timeout);
	}
	else
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
}
int	acquire_dongle(t_coder *coder, t_dongle *dongle, long arrival_time)
{
	t_request	request;
	t_request	removed;

	request = create_request(coder, arrival_time);
	pthread_mutex_lock(&dongle->mutex);
	if (!heap_push(&dongle->heap, request))
	{
		pthread_mutex_unlock(&dongle->mutex);
		return (0);
	}
	while (!simulation_stopped(coder->sim)
		&& !can_take(coder, dongle))
		wait_dongle(dongle);
	if (simulation_stopped(coder->sim))
	{
		heap_remove_coder(&dongle->heap, coder);
		pthread_mutex_unlock(&dongle->mutex);
		return (0);
	}
	heap_pop(&dongle->heap, &removed);
	dongle->available = 0;
	pthread_mutex_unlock(&dongle->mutex);
	return (1);
}

void	release_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->available = 1;
	dongle->available_at = get_time_ms()
		+ coder->sim->config.dongle_cooldown;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}