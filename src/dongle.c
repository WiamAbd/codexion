
#include "../codexion.h"


static t_request	create_request(t_coder *coder)
{
	t_request	request;

	request.coder = coder;
	request.arrival_time = get_time_ms();
	request.deadline = coder->last_compile_start
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

int	acquire_dongle(t_coder *coder, t_dongle *dongle)
{
	t_request	request;
	t_request	removed;

	request = create_request(coder);
	pthread_mutex_lock(&dongle->mutex);
	if (!heap_push(&dongle->heap, request))
	{
		pthread_mutex_unlock(&dongle->mutex);
		return (0);
	}
	while (!can_take(coder, dongle))
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
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