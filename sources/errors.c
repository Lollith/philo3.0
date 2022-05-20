/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agouet <agouet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 13:59:20 by agouet            #+#    #+#             */
/*   Updated: 2022/05/20 17:26:47 by agouet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	msg_error(char *error)
{
	write(1, error, ft_strlen (error));
	return (FAILURE);
}

int	msg_perror(char *origin)
{
	perror(origin);
	exit (EXIT_FAILURE);
}

void ft_free(t_rules rules)
{
		free(rules.th_id);
		free(rules.m_fork);
		free(rules.fork);
}

void ft_nb_meal(t_philo *philo)
{
		philo->nb_meal --;
		if (philo->nb_meal == 0)
		{
			pthread_mutex_lock(&philo->rules->m_one_die);
			philo->rules->all_eat --;
			pthread_mutex_unlock(&philo->rules->m_one_die);
		}

}