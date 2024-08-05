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


#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <memory.h>

#include "test-malloc-api-common.h"

#define BARRIER_HEIGHT 2
#define ALLOCATIONS_NUMBER 8
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef struct iterate_arg_s {
	uintptr_t allocs[ALLOCATIONS_NUMBER];
	size_t allocs_reported_number[ALLOCATIONS_NUMBER];
	size_t allocs_actual_sizes[ALLOCATIONS_NUMBER];
	size_t reported_sizes[ALLOCATIONS_NUMBER];
} iterate_arg_t;

typedef struct {
	uintptr_t *base;
	size_t size;
} allocations_info_t;

static const size_t allocs_sizes[ALLOCATIONS_NUMBER] = {
	8,
	2 * 1024,
	64 * 1024,
	512 * 1024,
	2 * 1024 * 1024,
	8 * 1024 * 1024,
	16 * 1024 * 1024,
	32 * 1024 * 1024
};

void iterate_callback(void *base, size_t size, void *data)
{
	iterate_arg_t *iterate_arg = (iterate_arg_t *) data;
	uintptr_t end;
	if (__builtin_add_overflow((uintptr_t) base, size, &end)) {
		return;
	}

	for (size_t i = 0; i < ALLOCATIONS_NUMBER; ++i) {
		if (iterate_arg->allocs[i] >= (uintptr_t) base && iterate_arg->allocs[i] < end) {
			iterate_arg->allocs_reported_number[i]++;
			uintptr_t max_size = end - iterate_arg->allocs[i];
			iterate_arg->reported_sizes[i] = MIN(size, max_size);
		}
	}
}

void fill_allocations_info(const iterate_arg_t *iterate_arg, allocations_info_t *allocations_info)
{
	size_t min_idx, max_idx;
	uintptr_t min_val = UINTPTR_MAX, max_val = 0;

	const uintptr_t *allocs = iterate_arg->allocs;

	for (size_t i = 0; i < ALLOCATIONS_NUMBER; ++i) {
		if (allocs[i] > max_val) {
			max_val = allocs[i];
			max_idx = i;
		}
		if (allocs[i] < min_val) {
			min_val = allocs[i];
			min_idx = i;
		}
	}

	allocations_info->base = (void *) allocs[min_idx];
	allocations_info->size = allocs[max_idx] - allocs[min_idx] + allocs_sizes[max_idx];
}

void make_allocations(iterate_arg_t *iterate_arg)
{
	uintptr_t *allocs = iterate_arg->allocs;
	size_t *allocs_actual_sizes = iterate_arg->allocs_actual_sizes;

	for (size_t i = 0; i < ALLOCATIONS_NUMBER; ++i) {
		allocs[i] = (uintptr_t) malloc(allocs_sizes[i]);
		allocs_actual_sizes[i] = malloc_usable_size((void *) allocs[i]);
	}
}

void free_allocations(iterate_arg_t *iterate_arg)
{
	uintptr_t *allocs = iterate_arg->allocs;

	for (size_t i = 0; i < ALLOCATIONS_NUMBER; ++i) {
		free((void *) allocs[i]);
	}
}

int iterate_wrapper(iterate_arg_t *iterate_arg)
{
	int ret = 0;
	allocations_info_t allocations_info;
	fill_allocations_info(iterate_arg, &allocations_info);
	malloc_iterate(allocations_info.base, allocations_info.size, iterate_callback, iterate_arg);

	for (size_t i = 0; i < ALLOCATIONS_NUMBER; ++i) {
		if (iterate_arg->allocs_reported_number[i] != 1) {
			ret = -1;
		}
	}
	return ret;
}

pthread_barrier_t routine_allocated;
pthread_barrier_t routine_iterated;

void *allocate_routine(void *vargp)
{
	iterate_arg_t *iterate_arg = (iterate_arg_t *) vargp;
	make_allocations(iterate_arg);
	pthread_barrier_wait(&routine_allocated);
	pthread_barrier_wait(&routine_iterated);
	return NULL;
}

void *abandoned_allocate_routine(void *vargp)
{
	iterate_arg_t *iterate_arg = (iterate_arg_t *) vargp;
	make_allocations(iterate_arg);
	return NULL;
}

int test_iterate_main_thread(void)
{
	int ret;
	iterate_arg_t iterate_arg = {{0}, {0}, {0}, {0}};
	make_allocations(&iterate_arg);
	ret = iterate_wrapper(&iterate_arg);
	free_allocations(&iterate_arg);
	return ret;
}

int test_iterate_another_thread(void)
{
	int ret;
	iterate_arg_t iterate_arg_routine = {{0}, {0}, {0}, {0}};
	pthread_barrier_init(&routine_allocated, NULL, BARRIER_HEIGHT);
	pthread_barrier_init(&routine_iterated, NULL, BARRIER_HEIGHT);
	pthread_t thread_id;
	pthread_create(&thread_id, NULL, allocate_routine, (void *) &iterate_arg_routine);
	pthread_barrier_wait(&routine_allocated);
	ret = iterate_wrapper(&iterate_arg_routine);
	free_allocations(&iterate_arg_routine);
	pthread_barrier_wait(&routine_iterated);
	return ret;
}

int test_iterate_over_abandoned_allocs(void)
{
	int ret;
	iterate_arg_t iterate_arg_routine = {{0}, {0}, {0}, {0}};
	pthread_t thread_id;
	pthread_create(&thread_id, NULL, abandoned_allocate_routine, (void *) &iterate_arg_routine);
	pthread_join(thread_id, NULL);
	ret = iterate_wrapper(&iterate_arg_routine);
	free_allocations(&iterate_arg_routine);
	return ret;
}

int main()
{
	int ret = 0;

	ret = check_and_report("Testing iterate main thread", test_iterate_main_thread);

	ret = -(ret || check_and_report("Testing iterate another thread", test_iterate_another_thread));

	ret = -(ret || check_and_report("Testing iterate over abandoned allocations", test_iterate_over_abandoned_allocs));

	return ret;
}
