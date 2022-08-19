/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agouet <agouet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 16:31:24 by agouet            #+#    #+#             */
/*   Updated: 2022/05/20 16:39:24 by agouet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_fork_eat(int *pt_left, int *pt_right, int *pt_num, t_philo *philo)
{
	int	left;
	int	right;
	int	num;
	int one_die;
	int all_eat;

	left = *pt_left;
	right = *pt_right;
	num = *pt_num;

	pthread_mutex_lock(&philo->rules->m_one_die);
	one_die = philo->rules->one_die;
	all_eat = philo->rules->all_eat;
	pthread_mutex_unlock(&philo->rules->m_one_die);
	

	while (!one_die && all_eat != 0)
	{
		if (left < right)
		{
			pthread_mutex_lock(&philo->rules->m_fork[left]);
			if(philo->rules->fork[left] == 1)
			{
					pthread_mutex_unlock(&philo->rules->m_fork[left]);
					pthread_mutex_lock(&philo->rules->m_one_die);
					one_die = philo->rules->one_die;
					pthread_mutex_unlock(&philo->rules->m_one_die);
					continue ;
			}
			else
			{
				pthread_mutex_lock(&philo->rules->m_fork[right]);
			
				if(philo->rules->fork[right] == 1)
				{
					pthread_mutex_unlock(&philo->rules->m_fork[right]);
					pthread_mutex_unlock(&philo->rules->m_fork[left]);
					pthread_mutex_lock(&philo->rules->m_one_die);
					one_die = philo->rules->one_die;
					pthread_mutex_unlock(&philo->rules->m_one_die);
					continue ;
				}
			}
		}
		else
		{
			pthread_mutex_lock(&philo->rules->m_fork[right]);
			if(philo->rules->fork[right] == 1)
				{
					pthread_mutex_unlock(&philo->rules->m_fork[right]);
				pthread_mutex_lock(&philo->rules->m_one_die);
				one_die = philo->rules->one_die;
				pthread_mutex_unlock(&philo->rules->m_one_die);
				continue ;
				}
			else
			{
				pthread_mutex_lock(&philo->rules->m_fork[left]);
			
				if(philo->rules->fork[left] == 1)
				{
					pthread_mutex_unlock(&philo->rules->m_fork[left]);
					pthread_mutex_unlock(&philo->rules->m_fork[right]);
					pthread_mutex_lock(&philo->rules->m_one_die);
					one_die = philo->rules->one_die;
					pthread_mutex_unlock(&philo->rules->m_one_die);
					continue ;
				}
			}
		}
			philo->rules->fork[left] = 1;
			philo->rules->fork[right] = 1;
			pthread_mutex_unlock(&philo->rules->m_fork[left]);
			pthread_mutex_unlock(&philo->rules->m_fork[right]);
			if (taking_fork(&num, philo->rules, philo) == 0)
				return (FAILURE);
			if (eating(&num, philo->rules, philo)== 0)
				return (FAILURE);
			pthread_mutex_lock(&philo->rules->m_fork[left]);
			philo->rules->fork[left] = 0;
			pthread_mutex_unlock(&philo->rules->m_fork[left]);
			pthread_mutex_lock(&philo->rules->m_fork[right]);
			philo->rules->fork[right] = 0;
			pthread_mutex_unlock(&philo->rules->m_fork[right]);
			return(SUCCESS);
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
	num = philo->num;
	left = num;
	right = (left + 1) % philo->rules->nb_philo;
//	pthread_mutex_lock(&philo->rules->m_one_die);
	one_die = philo->rules->one_die;
//	pthread_mutex_unlock(&philo->rules->m_one_die);
	if (num  == 0)
		usleep(philo->rules->t_eat * 2000);
	while (!one_die && philo->rules->all_eat != 0)
	{
		if(check_fork_eat(&left, &right, &num, philo) == 0)
			return (NULL);
		if(sleeping(&num, philo->rules, philo) == 0)
			return (NULL);
		if(thinking(&num, philo->rules, philo) == 0)
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

	long begin_eat;
	long state_diff;
	long actual_state;
	while (1)
	{
		temp = philo;
		while (temp && temp->num < temp->rules->nb_philo)
		{
			if (!check_die_meal(temp))
				return (NULL);
			pthread_mutex_lock(&temp->m_state);
			begin_eat = temp->begin_eat;
			pthread_mutex_unlock(&temp->m_state);
			actual_state = get_time() - philo->rules->time_ini;
			state_diff = actual_state - begin_eat;
			if (actual_state > begin_eat &&  state_diff > temp->rules->t_die )
			{
				pthread_mutex_lock(&philo->rules->m_one_die);
				philo->rules->one_die = 1;
				pthread_mutex_unlock(&philo->rules->m_one_die);
				dying(&temp->num, temp->rules, temp);
				return (NULL);
			}
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
