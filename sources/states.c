/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   states.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agouet <agouet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 16:47:34 by agouet            #+#    #+#             */
/*   Updated: 2022/05/20 17:27:30 by agouet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	taking_fork(int *pt_num, t_rules *rules, t_philo *philo)
{
	int		one_die;
	int		all_eat;
	long	state0;

	pthread_mutex_lock(&philo->rules->m_one_die);
	one_die = philo->rules->one_die;
	pthread_mutex_unlock(&philo->rules->m_one_die);
	all_eat = philo->rules->all_eat;
	if (!one_die && all_eat != 0)
	{
		state0 = get_time() - philo->rules->time_ini;
		pthread_mutex_lock(&philo->m_state);
		philo->t_take_fork = state0;
		pthread_mutex_unlock(&philo->m_state);
		pthread_mutex_lock(&rules->m_display);
		printf("%ld %d has taken a fork\n", state0, *pt_num + 1);
		printf("%ld %d has taken a fork\n", state0, *pt_num + 1);
		pthread_mutex_unlock(&rules->m_display);
		return (SUCCESS);
	}
	return (FAILURE);
}

int	eating(int *pt_num, t_rules *rules, t_philo *philo)
{
	long	state1;
	int		one_die;

	pthread_mutex_lock(&philo->rules->m_one_die);
	one_die = philo->rules->one_die;
	pthread_mutex_unlock(&philo->rules->m_one_die);
	if (!one_die && philo->rules->all_eat != 0)
	{
		state1 = get_time() - philo->rules->time_ini;
		pthread_mutex_lock(&philo->m_state);
		philo->begin_eat = state1;
		pthread_mutex_unlock(&philo->m_state);
		pthread_mutex_lock(&rules->m_display);
		printf("%ld %d is eating\n", state1, *pt_num + 1);
		pthread_mutex_unlock(&rules->m_display);
		ft_nb_meal(philo);
		usleep(rules->t_eat * 1000);
		return (SUCCESS);
	}
	return (FAILURE);
}

int	sleeping(int *pt_num, t_rules *rules, t_philo *philo)
{
	long	state2;
	int		all_eat;
	int		one_die;

	pthread_mutex_lock(&philo->rules->m_one_die);
	one_die = philo->rules->one_die;
	pthread_mutex_unlock(&philo->rules->m_one_die);
	all_eat = philo->rules->all_eat;
	if (!one_die && all_eat != 0)
	{
		state2 = get_time() - philo->rules->time_ini;
		pthread_mutex_lock(&rules->m_display);
		printf("%ld %d is sleeping\n", state2, *pt_num + 1);
		pthread_mutex_unlock(&rules->m_display);
		usleep(rules->t_sleep * 1000);
		return (SUCCESS);
	}
	return (FAILURE);
}

int	thinking(int *pt_num, t_rules *rules, t_philo *philo)
{
	long	state3;
	int		all_eat;
	int		one_die;

	pthread_mutex_lock(&philo->rules->m_one_die);
	one_die = philo->rules->one_die;
	pthread_mutex_unlock(&philo->rules->m_one_die);
	all_eat = philo->rules->all_eat;
	if (!one_die && all_eat != 0)
	{
		state3 = get_time() - philo->rules->time_ini;
		pthread_mutex_lock(&rules->m_display);
		printf("%ld %d is thinking\n", state3, *pt_num + 1);
		pthread_mutex_unlock(&rules->m_display);
		return (SUCCESS);
	}
	return (FAILURE);
}

void	dying(int *pt_num, t_rules *rules, t_philo *philo)
{
	int	one_die;

	pthread_mutex_lock(&philo->rules->m_one_die);
	one_die = philo->rules->one_die;
	pthread_mutex_unlock(&philo->rules->m_one_die);
	if (one_die == 1)
	{
		pthread_mutex_lock(&rules->m_display);
		printf("%ld ", get_time() - philo->rules->time_ini);
		printf("%d died\n", *pt_num +1);
		pthread_mutex_unlock(&rules->m_display);
	}
}
