/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agouet <agouet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 14:29:56 by agouet            #+#    #+#             */
/*   Updated: 2022/05/19 13:19:01 by agouet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long int get_time(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000 + (tv.tv_usec / 1000)));
}

void	one_philo(int *pt_num, t_philo *philo)
{
	int	num;

	num = *pt_num;
	printf("0 %d has taken a fork\n", num);
	pthread_mutex_lock(&philo->rules->m_one_die);
	philo->rules->one_die = 1;
	pthread_mutex_unlock(&philo->rules->m_one_die);
	usleep(philo->rules->t_eat * 1000);
	dying(&num, philo->rules, philo);
}

int	create_thread_p(t_rules *rules, t_philo *philo)
{
	int			i;
	pthread_t	id;
	t_philo		*temp;

	i = 0;
	temp = philo;
	while (i < rules->nb_philo && temp)
	{
		if (pthread_create(&rules->th_id[i], NULL, &routine_philo, temp) != 0)
			return (msg_error("error pthread_create\n"));
		temp = temp->next;
		i++;
	}
	if (pthread_create(&id, NULL, &reaper, philo) != 0)
		return (msg_error("error pthread_create\n"));
	i = 0;
	while (i < rules->nb_philo)
	{
		pthread_join(rules->th_id[i], NULL);
		i++;
	}
	pthread_join(id, NULL);
	return (SUCCESS);
}
/*
int check_usleep(int time, t_philo *philo)
{
	int few_time;
	int i;
	int one_die;
	i = 1;
	few_time= time / 10;
	while (i <= 10)
	{
		usleep(few_time * 1000);
		pthread_mutex_lock(&philo->rules->m_one_die);
		one_die = philo->rules->one_die;
		pthread_mutex_unlock(&philo->rules->m_one_die);
		if (one_die)
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}*/