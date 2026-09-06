/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: horarivo <horarivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 22:33:55 by horarivo          #+#    #+#             */
/*   Updated: 2026/09/06 09:42:25 by horarivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <time.h>

typedef struct s_data		t_data;
typedef struct s_dongle		t_dongle;
typedef struct s_coder		t_coder;
typedef enum e_state		t_state;
typedef struct s_sim		t_sim;
typedef struct s_wait_node	t_wait_node;

typedef enum e_state
{
	WAITING,
	COMPILING,
	DEBUGGING,
	REFACTORING
}	t_state;

typedef struct s_sim
{
	int	number_of_coders;
	int	time_to_burnout;
	int	time_to_compile;
	int	time_to_debug;
	int	time_to_refactor;
	int	number_of_compiles_required;
	int	dongle_cooldown;
	int	scheduler;
}	t_sim;

typedef struct s_wait_node
{
	long long	key;
	int			coder_id;
}	t_wait_node;

typedef struct s_heap
{
	t_wait_node	*nodes;
	int			size;
	int			capacity;
}	t_heap;

typedef struct s_dongle
{
	int				id_dongle;
	int				is_taken;
	long long		last_release;
	t_heap			wait_queue;
	pthread_mutex_t	dongle_mutex;
	pthread_cond_t	dongle_cond;
}	t_dongle;

typedef struct s_coder
{
	int			id_coder;
	int			compiles_done;
	long long	last_compile_start;
	t_dongle	*l_dongle;
	t_dongle	*r_dongle;
	t_state		state;
	t_data		*data;
	pthread_t	coder_thread;
}	t_coder;

typedef struct s_data
{
	t_sim			*params;
	t_coder			*coders;
	t_dongle		*dongles;
	long long		start_time;
	int				stop;
	pthread_t		monitor_thread;
	pthread_mutex_t	log_mutex;
	pthread_mutex_t	state_mutex;
}	t_data;

int			parser(t_sim *sim, char **argv);
int			init_data(t_data *data);
int			start_simulation(t_data *data);

void		build_timeout(struct timespec *ts);
long long	get_time_ms(void);

int			acquire_dongles(t_coder *coder);
int			take_dongle(t_coder *coder, t_dongle *d);
void		release_dongle(t_dongle *d);
void		wake_all_dongles(t_data *data);

void		*routine(void *arg);
int			check_all_coders(t_data *data, int n, int required);
void		*monitor_routine(void *arg);

void		log_state(t_data *data, int id, char *msg);

void		cleanup(t_data *data);

void		heap_push(t_heap *h, long long key, int coder_id);
int			heap_pop(t_heap *h);
int			heap_peek(t_heap *h);
void		heap_remove(t_heap *h, int coder_id);

long long	compute_key(t_coder *coder);

int			dongle_available(t_dongle *d, t_data *data);
int			get_stop(t_data *data);

int			has_priority(t_wait_node a, t_wait_node b);

#endif
