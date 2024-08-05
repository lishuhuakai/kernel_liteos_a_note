#include <malloc.h>
#include <time.h>
#include <sched.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include "test.h"

#define THREAD_MAX_N 8
#define SIZE_ALIGN (4 * sizeof(size_t))
#define MMAP_THRESHOLD 131052
#define FREE_CYCLE 16
#define THRESHOLD (MMAP_THRESHOLD / 16)
#define ITER_TIME 80
#define NANOSEC_PER_SEC 1e9
#define MALLOC_TIME (ITER_TIME * (THRESHOLD / (SIZE_ALIGN + 1)))

void free_all(void **ptr)
{
	for (int j = 0; j < FREE_CYCLE; j++) {
		free(ptr[j]);
	}
}

void *func(void *arg)
{
	int *val = (int *)arg;
	cpu_set_t mask;
	struct timespec ts[2];
	int num = 0;
	void *ptr[FREE_CYCLE];

	CPU_ZERO(&mask);
	CPU_SET(0, &mask);
	if (sched_setaffinity(0, sizeof(mask), &mask) < 0) {
		t_error("Set CPU affinity of thread %d failure, ERROR:%s\n", *val, strerror(errno));
		return NULL;
	}

	for (int i = 0; i < ITER_TIME; ++i) {
		for (size_t size = 0; size < THRESHOLD; size += SIZE_ALIGN + 1) {
			if (num == FREE_CYCLE) {
				free_all(ptr);
				num = 0;
			}
			ptr[num] = malloc(size);
			if (!ptr[num]) {
				t_error("Thread %d malloc failed for size %u\n", *val, size);
				*val = errno;
				return NULL;
			}
			num++;
		}
	}

	*val = 0;
	return NULL;
}

int main(int argc, char *argv[])
{
	struct timespec ts[2];
	pthread_attr_t attr;
	pthread_t tids[THREAD_MAX_N];
	int t_result[THREAD_MAX_N] = {0};
	int flag = 0;
	int ret;
	int i;

	ret = pthread_attr_init(&attr);
	if (ret < 0) {
		t_error("Init pthread attribute failed: %s\n", strerror(errno));
		return -1;
	}

	clock_gettime(CLOCK_REALTIME, ts);
	for (i = 0; i < THREAD_MAX_N; ++i) {
		t_result[i] = i;
		ret = pthread_create(&tids[i], &attr, func, &t_result[i]);
		if (ret < 0) {
			t_error("Create pthread %u failed: %s\n", i, strerror(errno));
			flag = -1;
			break;
		}
	}

	for (i = 0; i < THREAD_MAX_N; ++i) {
		ret = pthread_join(tids[i], NULL);
		if (ret < 0) {
			t_error("Join thread %u failed: %s\n", i, strerror(errno));
		}
	}
	clock_gettime(CLOCK_REALTIME, ts + 1);

	(void)pthread_attr_destroy(&attr);

	double cost = (ts[1].tv_sec - ts[0].tv_sec) * NANOSEC_PER_SEC +
		(ts[1].tv_nsec - ts[0].tv_nsec);

	if (!flag) {
		t_printf("Malloc and free %d threads %d times cost %lf s\n", THREAD_MAX_N, MALLOC_TIME, cost / NANOSEC_PER_SEC);
		t_status = 0;
	}

	return t_status;
}
