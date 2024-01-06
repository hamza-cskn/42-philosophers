#include <sys/time.h>
#define USEC_IN_SEC 1000000

struct timeval get_cur_time() {
	struct timeval	current_time;
	gettimeofday(&current_time, NULL);
	return current_time;
}

struct timeval time_diff(struct timeval start, struct timeval end)
{
	struct timeval diff;

	diff.tv_sec = end.tv_sec - start.tv_sec;
	diff.tv_usec = end.tv_usec - start.tv_usec;
	if (diff.tv_usec < 0)
	{
		diff.tv_sec--;
		diff.tv_usec += 1000000;
	}
	return (diff);
}

struct timeval to_timeval(long long ms) {
	long long usec = ms * 1e3;
	return (struct timeval) { .tv_sec = 0 , .tv_usec = usec};
}

