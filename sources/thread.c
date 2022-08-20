/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agouet <agouet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 16:31:24 by agouet            #+#    #+#             */
/*   Updated: 2022/08/20 17:45:13 by agouet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_the_good_one(t_philo *philo, int one_die, int side, int o_side)
{
	pthread_mutex_lock(&philo->rules->m_fork[side]);
	if (philo->rules->fork[side] == 1)
	{
		pthread_mutex_unlock(&philo->rules->m_fork[side]);
		pthread_mutex_lock(&philo->rules->m_one_die);
		one_die = philo->rules->one_die;
		pthread_mutex_unlock(&philo->rules->m_one_die);
	}
	else
	{
		pthread_mutex_lock(&philo->rules->m_fork[o_side]);
		if (philo->rules->fork[o_side] == 1)
		{
			pthread_mutex_unlock(&philo->rules->m_fork[o_side]);
			pthread_mutex_unlock(&philo->rules->m_fork[side]);
			pthread_mutex_lock(&philo->rules->m_one_die);
			one_die = philo->rules->one_die;
			pthread_mutex_unlock(&philo->rules->m_one_die);
		}
	}
}

void	check_impair_wait(t_philo *philo)
{
	long	t;

	if (philo->rules->nb_philo % 2 != 0)
	{
		t = 2 * philo->rules->t_eat + 10 + philo->t_take_fork;
		while (philo->t_take_fork > 1
			&& get_time() - philo->rules->time_ini < t)
			usleep (2000);
	}
}

int	check_fork_eat(int *pt_left, int *pt_right, int *pt_num, t_philo *philo)
{
	int	left;
	int	right;
	int	num;
	int	one_die;
	int	all_eat;

	left = *pt_left;
	right = *pt_right;
	num = *pt_num;
	pthread_mutex_lock(&philo->rules->m_one_die);
	one_die = philo->rules->one_die;
	all_eat = philo->rules->all_eat;
	pthread_mutex_unlock(&philo->rules->m_one_die);
	while (!one_die && all_eat != 0)
	{
		check_impair_wait(philo);
		if (left < right)
			take_the_good_one(philo, one_die, left, right);
		else
			take_the_good_one(philo, one_die, right, left);
		ready_to_eat(philo, left, right, num);
		return (SUCCESS);
	}
	return (FAILURE);
}

int	ready_to_eat(t_philo *philo, int left, int right, int num)
{
	pthread_mutex_unlock(&philo->rules->m_fork[left]);
	pthread_mutex_unlock(&philo->rules->m_fork[right]);
	if (taking_fork(&num, philo->rules, philo) == 0)
		return (FAILURE);
	if (eating(&num, philo->rules, philo) == 0)
		return (FAILURE);
	pthread_mutex_lock(&philo->rules->m_fork[left]);
	philo->rules->fork[left] = 0;
	pthread_mutex_unlock(&philo->rules->m_fork[left]);
	pthread_mutex_lock(&philo->rules->m_fork[right]);
	philo->rules->fork[right] = 0;
	pthread_mutex_unlock(&philo->rules->m_fork[right]);
	return (SUCCESS);
}

// num = left et nb;
void	*routine_philo(void *arg)
{
	t_philo	*philo;
	int		one_die;
	int		num;
	int		right;

	philo = (t_philo *)arg;
	num = philo->num;
	philo->t_take_fork = 0;
	right = (num + 1) % philo->rules->nb_philo;
	one_die = philo->rules->one_die;
	if (num % 2 == 0)
		usleep(philo->rules->t_eat * 1000);
	while (!one_die && philo->rules->all_eat != 0)
	{
		if (check_fork_eat(&num, &right, &num, philo) == 0)
			return (NULL);
		if (sleeping(&num, philo->rules, philo) == 0)
			return (NULL);
		if (thinking(&num, philo->rules, philo) == 0)
			return (NULL);
		pthread_mutex_lock(&philo->rules->m_one_die);
		one_die = philo->rules->one_die;
		pthread_mutex_unlock(&philo->rules->m_one_die);
	}
	return (NULL);
}
