#ifndef CODEXION_H
# define CODEXION_H

# include <limits.h>
# include <string.h>
# include <pthread.h>
# include <stdlib.h>
# include <sys/time.h>

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
	long		id;
	pthread_t	thread;
	long		last_compile_start;
	long		compile_count;
    t_sim       *sim;
}	t_coder;

typedef struct s_dongle
{
	int				available;
	long			available_at;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
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

int	    parse_arguments(int argc, char **argv, t_config *config);
int	    init_simulation(t_sim *sim);
int		parse_arguments(int argc, char **argv, t_config *config);
int		init_simulation(t_sim *sim);
void	cleanup_simulation(t_sim *sim);
void	destroy_dongles(t_sim *sim, long count);
long	get_time_ms(void);
long	get_elapsed_ms(t_sim *sim);

#endif