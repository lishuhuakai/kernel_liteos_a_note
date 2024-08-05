/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef TEST_MALLOC_STATS_COMMON_H
#define TEST_MALLOC_STATS_COMMON_H

#define _GNU_SOURCE

#include <unistd.h>
#include <sys/syscall.h>
#include <stddef.h>
#include <pthread.h>
#include <malloc.h>
#include "test.h"

#define SIZES_COUNT 10
#define SIZE_ALIGN (8 * sizeof(size_t))
#define MMAP_THRESHOLD ((0x1c00 * SIZE_ALIGN) - OVERHEAD)
#define LIST_OVERHEAD (2 * sizeof(void *))
#define OVERHEAD (sizeof(size_t) + LIST_OVERHEAD)

static size_t sizes[SIZES_COUNT] = {
	23,
	32,
	256,
	3072,
	3584,
	262144,
	327680,
	8 * 1024 * 1024,
	16 * 1024 * 1024,
	32 * 1024 * 1024
};

typedef struct {
	long long mmapped_regions;
	long long total_mmapped_memory;
	long long total_allocated_memory;
} malloc_thread_stats_t;

typedef struct {
	size_t alloc_size;
	pthread_barrier_t *alloc_barrier;
	pthread_barrier_t *free_barrier;
	pid_t self_id;
} thread_data_t;

static malloc_thread_stats_t get_total_from_test_sizes()
{
	malloc_thread_stats_t total_stats = {0};
	for (size_t i = 0; i < SIZES_COUNT; i++) {
		if (sizes[i] > MMAP_THRESHOLD) {
			total_stats.total_mmapped_memory += sizes[i];
			total_stats.mmapped_regions++;
		}
		total_stats.total_allocated_memory += sizes[i];
	}
	return total_stats;
}

static int expect_greater_equal(long long amt1, long long amt2, const char *amt1_name, const char *amt2_name)
{
	if (amt1 >= amt2) {
		return 1;
	}
	t_error("Expected %s(value: %lld) to be >= %s(value: %lld)\n", amt1_name, amt1, amt2_name, amt2);
	return 0;
}

static int expect_equal(long long amt, long long value, const char *amt_name)
{
	if (amt == value) {
		return 1;
	}
	t_error("Expected %s(value: %lld) to be %lld\n", amt_name, amt, value);
	return 0;
}

static int validate_total_allocated(malloc_thread_stats_t *total_stats)
{
	malloc_thread_stats_t total_from_test_sizes = get_total_from_test_sizes();

	int result = expect_greater_equal(
		total_stats->total_allocated_memory,
		total_from_test_sizes.total_allocated_memory,
		"allocated memory",
		"total memory from test sizes");
	result &= expect_greater_equal(
		total_stats->total_mmapped_memory,
		total_from_test_sizes.total_mmapped_memory,
		"mmapped memory",
		"total large memory from test sizes");
	result &= expect_equal(
		total_stats->mmapped_regions,
		total_from_test_sizes.mmapped_regions,
		"mmapped regions");
	return result;
}

static int validate_all_freed(malloc_thread_stats_t *total_stats)
{
	int result = expect_equal(total_stats->total_allocated_memory, 0, "allocated memory");
	result &= expect_equal(total_stats->total_mmapped_memory, 0, "mmapped memory");
	result &= expect_equal(total_stats->mmapped_regions, 0, "mmapped regions");
	return result;
}

static void *allocate_wait_free(void *arg)
{
	thread_data_t *thread_data = arg;
	thread_data->self_id = syscall(__NR_gettid);
	void *alloc = malloc(thread_data->alloc_size);
	pthread_barrier_wait(thread_data->alloc_barrier);
	pthread_barrier_wait(thread_data->free_barrier);
	free(alloc);
	return NULL;
}

#endif // TEST_MALLOC_STATS_COMMON_H
