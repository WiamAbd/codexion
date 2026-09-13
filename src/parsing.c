/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wabdella <wabdella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 01:22:07 by wabdella          #+#    #+#             */
/*   Updated: 2026/09/13 01:22:07 by wabdella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static int	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

static int	is_positive_number(char *str)
{
	int	i;
	int	digits;

	if (!str || !str[0])
		return (0);
	i = 0;
	while ((9 <= str[i] && str[i] <= 13) || str[i] == 32)
		i++;
	digits = 0;
	while (is_digit(str[i]))
	{
		digits++;
		i++;
	}
	while ((9 <= str[i] && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] != '\0' || digits == 0)
		return (0);
	return (1);
}

static int	ascii_to_long(char *str, long *result)
{
	long	number;
	int		i;

	if (!is_positive_number(str))
		return (0);
	number = 0;
	i = 0;
	while ((9 <= str[i] && str[i] <= 13) || str[i] == 32)
		i++;
	while (is_digit(str[i]))
	{
		if (number > (LONG_MAX - (str[i] - '0')) / 10)
			return (0);
		number = number * 10 + (str[i] - '0');
		i++;
	}
	if (number > INT_MAX)
		return (0);
	*result = number;
	return (1);
}

static int	parse_numbers(char **argv, t_config *config)
{
	if (!ascii_to_long(argv[1], &config->number_of_coders))
		return (0);
	if (!ascii_to_long(argv[2], &config->time_to_burnout))
		return (0);
	if (!ascii_to_long(argv[3], &config->time_to_compile))
		return (0);
	if (!ascii_to_long(argv[4], &config->time_to_debug))
		return (0);
	if (!ascii_to_long(argv[5], &config->time_to_refactor))
		return (0);
	if (!ascii_to_long(argv[6], &config->number_of_compiles_required))
		return (0);
	if (!ascii_to_long(argv[7], &config->dongle_cooldown))
		return (0);
	return (1);
}

int	parse_arguments(int argc, char **argv, t_config *config)
{
	if (argc != 9)
		return (0);
	if (!parse_numbers(argv, config))
		return (0);
	if (strcmp(argv[8], "fifo") != 0
		&& strcmp(argv[8], "edf") != 0)
		return (0);
	config->scheduler = argv[8];
	return (1);
}
