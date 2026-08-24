#ifndef CODEXION_H
# define CODEXION_H

# include <limits.h>
# include <string.h>

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

int	parse_arguments(int argc, char **argv, t_config *config);

#endif