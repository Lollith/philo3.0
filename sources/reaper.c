/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reaper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lollith <lollith@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 22:00:02 by lollith           #+#    #+#             */
/*   Updated: 2022/08/19 22:01:58 by lollith          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_t_to_die(t_philo *temp)
{
	long	actual_state;
	long	state_diff;
	long	begin_eat;

	pthread_mutex_lock(&temp->m_state);
	begin_eat = temp->begin_eat;
	pthread_mutex_unlock(&temp->m_state);
	actual_state = get_time() - temp->rules->time_ini;
	state_diff = actual_state - begin_eat;
	if (actual_state > begin_eat && state_diff > temp->rules->t_die)
	{
		pthread_mutex_lock(&temp->rules->m_one_die);
		temp->rules->one_die = 1;
		pthread_mutex_unlock(&temp->rules->m_one_die);
		dying(&temp->num, temp->rules, temp);
		return ;
	}
}

void	*reaper(void *arg)
{
	t_philo	*temp;
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		temp = philo;
		while (temp && temp->num < temp->rules->nb_philo)
		{
			if (!check_die_meal(temp))
				return (NULL);
			check_t_to_die(temp);
			temp = temp->next;
		}
		usleep(8 * 1000);
	}
	return (NULL);
}

int	check_die_meal(t_philo *philo)
{
	int	one_die;
	int	all_eat;

	pthread_mutex_lock(&philo->rules->m_one_die);
	one_die = philo->rules->one_die;
	all_eat = philo->rules->all_eat;
	pthread_mutex_unlock(&philo->rules->m_one_die);
	if (one_die || all_eat == 0)
		return (FAILURE);
	return (SUCCESS);
}
