/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agouet <agouet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 14:29:56 by agouet            #+#    #+#             */
/*   Updated: 2022/08/20 17:44:45 by agouet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long int	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000 + (tv.tv_usec / 1000)));
}

void	one_philo(int *pt_num, t_philo *philo)
{
	int	num;

	num = *pt_num - 1;
	printf("0 %d has taken a fork\n", num + 1);
	pthread_mutex_lock(&philo->rules->m_one_die);
	philo->rules->one_die = 1;
	pthread_mutex_unlock(&philo->rules->m_one_die);
	usleep(philo->rules->t_eat * 1000);
	dying(&num, philo->rules, philo);
}

int	create_thread_p(t_rules *rules, t_philo *philo)
{
	int			i;
	t_philo		*temp;
	pthread_t	id;

	i = 0;
	temp = philo;
	if (philo->rules->nb_philo == 1)
	{
		one_philo(&philo->rules->nb_philo, philo);
		return (FAILURE);
	}
	while (i < rules->nb_philo && temp)
	{
		if (pthread_create(&rules->th_id[i], NULL, &routine_philo, temp) != 0)
			return (msg_error("error pthread_create\n"));
		temp = temp->next;
		i++;
	}
	if (pthread_create(&id, NULL, &reaper, philo) != 0)
		return (msg_error("error pthread_create\n"));
	ft_wait_thread(rules, &id);
	return (SUCCESS);
}

void	ft_wait_thread( t_rules *rules, pthread_t *id)
{
	int		i;

	i = 0;
	while (i < rules->nb_philo)
	{
		pthread_join(rules->th_id[i], NULL);
		i++;
	}
	pthread_join(*id, NULL);
}
