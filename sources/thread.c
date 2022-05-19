/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agouet <agouet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 16:31:24 by agouet            #+#    #+#             */
/*   Updated: 2022/05/19 15:06:40 by agouet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_fork_eat(int *pt_left, int *pt_right, int *pt_num, t_philo *philo)
{
	int	left;
	int	right;
	int	num;
	int one_die;

	left = *pt_left;
	right = *pt_right;
	num = *pt_num;

	pthread_mutex_lock(&philo->rules->m_one_die);
	one_die = philo->rules->one_die;
	pthread_mutex_unlock(&philo->rules->m_one_die);
	

	while (!one_die && philo->rules->all_eat != 0)
	{
		if (left < right)
		{
			pthread_mutex_lock(&philo->rules->m_fork[left]);
			pthread_mutex_lock(&philo->rules->m_fork[right]);
		}
		else
		{
			pthread_mutex_lock(&philo->rules->m_fork[right]);
			pthread_mutex_lock(&philo->rules->m_fork[left]);
		}
		if (philo->rules->fork[left] == 0 && philo->rules->fork[right] == 0)
		{
			philo->rules->fork[left] = 1;
			philo->rules->fork[right] = 1;
			pthread_mutex_unlock(&philo->rules->m_fork[left]);
			pthread_mutex_unlock(&philo->rules->m_fork[right]);
			if (taking_fork(&num, philo->rules, philo) == 0)
				return (FAILURE);
			if (eating(&num, philo->rules, philo)== 0)
				return (FAILURE);
			pthread_mutex_lock(&philo->rules->m_fork[right]);
			pthread_mutex_lock(&philo->rules->m_fork[left]);
			philo->rules->fork[left] = 0;
			philo->rules->fork[right] = 0;
			pthread_mutex_unlock(&philo->rules->m_fork[left]);
			pthread_mutex_unlock(&philo->rules->m_fork[right]);
			return(SUCCESS);
		}
		else 	
		{

			pthread_mutex_unlock(&philo->rules->m_fork[left]);
			pthread_mutex_unlock(&philo->rules->m_fork[right]);
			pthread_mutex_lock(&philo->rules->m_one_die);
			one_die = philo->rules->one_die;
			pthread_mutex_unlock(&philo->rules->m_one_die);
		}
	}
	return (FAILURE);
}

void	*routine_philo(void *arg)
{
	t_philo	*philo;
	int		one_die;
	int		num;
	int		left;
	int		right;

	philo = (t_philo *)arg;
	if (philo->rules->nb_philo == 1)
	{
		one_philo(&philo->rules->nb_philo, philo);
		return (NULL);
	}
	num = philo->num + 1;
	left = num - 1;
	right = (left + 1) % philo->rules->nb_philo;
	pthread_mutex_lock(&philo->rules->m_one_die);
	one_die = philo->rules->one_die;
	pthread_mutex_unlock(&philo->rules->m_one_die);
	if (philo->num % 2 == 0)
		usleep(philo->rules->t_eat * 1000);
	while (!one_die && philo->rules->all_eat != 0)
	{
		if(check_fork_eat(&left, &right, &num, philo) == 0)
			return (NULL);
		if(sleeping(&num, philo->rules, philo) == 0)
			return (NULL);
		if(thinking(&num, philo->rules, philo)== 0)
			return (NULL);
		pthread_mutex_lock(&philo->rules->m_one_die);
		one_die = philo->rules->one_die;
		pthread_mutex_unlock(&philo->rules->m_one_die);
	}
	return (NULL);
}

void	*reaper(void *arg)
{
	t_philo	*temp;
	t_philo	*philo;

	philo = (t_philo *)arg;

	while (1)
	{
		temp = philo;
		if (!check_die_meal(philo))
			return (NULL);
		if(!check_t_die(temp, philo))
			return (NULL);
	}
	return NULL;
}

int	check_t_die(t_philo *temp, t_philo *philo)
{
	int	one_die;
	long state[4]; // suprimer mes mutex sur state[i]
	long state_diff;
	long actual_state;

	while (temp && temp->num < temp->rules->nb_philo)
	{
		
		while (1)
		{
			pthread_mutex_lock(&temp->m_state[1]);
			state[1] = temp->state[1];
			pthread_mutex_unlock(&temp->m_state[1]);
			actual_state = get_time() - philo->rules->time_ini;
			state_diff = actual_state - state[1];
			if (actual_state > state[1] &&  state_diff > temp->rules->t_die )
			{
				one_die = 1;
				pthread_mutex_lock(&philo->rules->m_one_die);
				philo->rules->one_die = one_die;
				pthread_mutex_unlock(&philo->rules->m_one_die);
				dying(&temp->num, temp->rules, temp);
				return (FAILURE);
			}
		}
		temp = temp->next;
	}
	return (SUCCESS);
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
