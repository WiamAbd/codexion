#ifndef CODEXION_H
# define CODEXION_H

# include <limits.h>
# include <string.h>
# include <pthread.h>
# include <stdlib.h>
# include <sys/time.h>
# include <time.h>
# include <unistd.h>
# include <stdio.h>

typedef struct s_config
{
	long	number_of_coders;
	long	time_to_burnout;
	long	time_to_compile;
	long	time_to_debug;
	long	time_to_refactor;
	long	number_of_compiles_required;
	long	dongle_cooldown;
	char	*scheduler;
}	t_config;

typedef struct s_sim	t_sim;
typedef struct s_coder
{
	long			id;
	pthread_t		thread;
	long			last_compile_start;
	long			compile_count;
	pthread_mutex_t	state_mutex;
	t_sim			*sim;
}	t_coder;
typedef struct s_request
{
	t_coder	*coder;
	long	arrival_time;
	long	deadline;
}	t_request;

typedef struct s_heap
{
	t_request	*items;
	long		size;
	t_sim		*sim;
}	t_heap;
typedef struct s_dongle
{
	int				available;
	long			available_at;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	t_heap			heap;
}	t_dongle;

typedef struct s_sim
{
	t_config	config;
	t_coder		*coders;
	t_dongle	*dongles;
	long		start_time;
	int			stop;
	pthread_t	monitor;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	state_mutex;
}	t_sim;



int			parse_arguments(int argc, char **argv, t_config *config);
int			init_simulation(t_sim *sim);
void		cleanup_simulation(t_sim *sim);
void		destroy_dongles(t_sim *sim, long count);
long		get_time_ms(void);
long		get_elapsed_ms(t_sim *sim);
int			init_heap(t_heap *heap, t_sim *sim);
int			heap_push(t_heap *heap, t_request request);
t_request	*heap_peek(t_heap *heap);
int			heap_pop(t_heap *heap, t_request *result);
void		destroy_heap(t_heap *heap);
void		sift_up(t_heap *heap, long index);
void		sift_down(t_heap *heap, long index);
void		release_dongle(t_coder *coder, t_dongle *dongle);
int			acquire_dongle(t_coder *coder, t_dongle *dongle);
void		ms_to_timespec(long ms, struct timespec *time);
void		heap_repair(t_heap *heap, long index);
int			heap_remove_coder(t_heap *heap, t_coder *coder);
int			simulation_stopped(t_sim *sim);
void		stop_simulation(t_sim *sim);
void		destroy_coders(t_sim *sim, long count);
void		sim_sleep(long duration, t_sim *sim);
void		*coder_routine(void *arg);
void		log_state(t_coder *coder, char *state);
void		log_burnout(t_coder *coder);
void		*monitor_routine(void *arg);
int			start_simulation(t_sim *sim);

#endif