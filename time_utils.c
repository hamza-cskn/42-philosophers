/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcoskun <hcoskun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 19:10:09 by hcoskun           #+#    #+#             */
/*   Updated: 2024/01/30 20:44:15 by hcoskun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "philo.h"

struct timeval	get_cur_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return (current_time);
}

struct timeval	time_diff(struct timeval start, struct timeval end)
{
	struct timeval	diff;

	diff.tv_sec = end.tv_sec - start.tv_sec;
	diff.tv_usec = end.tv_usec - start.tv_usec;
	if (diff.tv_usec < 0)
	{
		diff.tv_sec--;
		diff.tv_usec += 1000000;
	}
	return (diff);
}

struct timeval	to_timeval(unsigned long long ms)
{
	long long	usec;

	usec = ms * 1000;
	return ((struct timeval){.tv_sec = 0, .tv_usec = (int) usec});
}
void	suspend_thread(struct timeval time)
{
	struct timeval		start;
	unsigned long long	time_usec;
	unsigned long long	start_usec;
	unsigned long long	cur_usec;
	struct timeval		cur;

	time_usec = time.tv_sec * 1000000 + time.tv_usec;
	start = get_cur_time();
	start_usec = start.tv_sec * 1000000 + start.tv_usec;
	cur_usec = 0;
	usleep(time_usec - 10000);
	cur = get_cur_time();
	cur_usec = cur.tv_sec * 1000000 + cur.tv_usec;
	while (cur_usec - start_usec < time_usec)
	{
		cur = get_cur_time();
		cur_usec = cur.tv_sec * 1000000 + cur.tv_usec;
	}
}

unsigned long long	get_timestamp(struct timeval start)
{
	struct timeval	diff;

	diff = time_diff(start, get_cur_time());
	return (diff.tv_sec * 1000 + diff.tv_usec / 1000);
}
