/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lollith <lollith@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 11:59:26 by agouet            #+#    #+#             */
/*   Updated: 2022/08/19 22:48:09 by lollith          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>//exit
# include <sys/time.h>

# define FAILURE 0
# define SUCCESS 1

typedef struct s_rules
{
	int				nb_t_must_eat;
	int				t_eat;
	int				t_sleep;
	int				t_die;
	int				nb_philo;
	int				all_eat;
	long int		time_ini;
	pthread_mutex_t	*m_fork;
	int				*fork;
	pthread_mutex_t	m_display;
	pthread_t		*th_id;
	int				one_die;
	pthread_mutex_t	m_one_die;
}					t_rules;

typedef struct s_philo
{
	int				num;
	int				nb_meal;
	long			begin_eat;
	long			t_take_fork;
	pthread_mutex_t	m_state;
	t_rules			*rules;
	struct s_philo	*next;
}					t_philo;

/*----------------------------------------------------------------------------*/
/*									MAIN									  */
/*----------------------------------------------------------------------------*/
int			initiate(t_rules *rules);
int			finisher(t_philo *philo);
int			ft_check_digit( int ac, char **av);
void		ft_input(t_rules *rules, char **argv);

/*----------------------------------------------------------------------------*/
/*									UTILS									  */
/*----------------------------------------------------------------------------*/
int			ft_isdigit(int c);
size_t		ft_strlen(const char *s);
int			ft_atoi(const char *nptr);
void		*ft_memset(void *s, int c, size_t n);
void		*ft_calloc(size_t nmemb, size_t size);

/*----------------------------------------------------------------------------*/
/*								LIST_PHILO									  */
/*----------------------------------------------------------------------------*/
int			list_philo(t_rules *rules, t_philo **philo);
t_philo		*ft_lstnew(int content, t_rules *rules);
t_philo		*ft_lstlast(t_philo *lst);
void		ft_lstadd_back(t_philo **alst, t_philo *new);
void		ft_lstclear(t_philo **lst);

/*----------------------------------------------------------------------------*/
/*									THREAD									  */
/*----------------------------------------------------------------------------*/
int			check_fork_eat(int *left, int *right, int *pt_num, t_philo *p);	
void		*routine_philo(void *arg);
int			check_t_die(t_philo *temp, t_philo *philo);

int			ready_to_eat(t_philo *philo, int left, int right, int num);
/*----------------------------------------------------------------------------*/
/*									REAPER									  */
/*----------------------------------------------------------------------------*/
void		check_t_to_die(t_philo *temp);
void		*reaper(void *arg);
int			check_die_meal(t_philo *philo);

/*----------------------------------------------------------------------------*/
/*									STATES									  */
/*----------------------------------------------------------------------------*/
int			taking_fork(int *pt_num, t_rules *rules, t_philo *philo);
int			eating( int *pt_num, t_rules *rules, t_philo *philo);
int			sleeping(int *pt_num, t_rules *rules, t_philo *philo);
int			thinking(int *pt_num, t_rules *rules, t_philo *philo);
void		dying(int *pt_num, t_rules *rules, t_philo *philo);

/*----------------------------------------------------------------------------*/
/*									TIME									  */
/*----------------------------------------------------------------------------*/
long int	get_time(void);
int			lck_ulck(pthread_mutex_t m_one_die, int philo_rules_one_die);
void		one_philo(int *pt_num, t_philo *philo);
int			create_thread_p(t_rules *rules, t_philo *philo);
void		ft_wait_thread( t_rules *rules, pthread_t *id);

/*----------------------------------------------------------------------------*/
/*									ERRORS									  */
/*----------------------------------------------------------------------------*/
int			msg_perror(char *origin);
int			msg_error(char *error);
int			ft_free(t_rules rules);
void		ft_nb_meal(t_philo *philo);

#endif
