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

#include "test-malloc-stats-common.h"

static int stats_from_mallinfo(malloc_thread_stats_t *stats, long long *total_free_heap_space, int use_mallinfo2)
{
	if (use_mallinfo2) {
		struct mallinfo2 info = mallinfo2();
		*stats = (malloc_thread_stats_t) {info.hblks, info.hblkhd, info.uordblks};
		*total_free_heap_space = info.fordblks;
		return 1;
	}
	struct mallinfo info = mallinfo();
	if (info.hblks < 0 || info.hblkhd < 0 || info.uordblks < 0 || info.fordblks < 0) {
		t_error("struct mallinfo contains negative numbers\n");
		return 0;
	}
	*stats = (malloc_thread_stats_t) {info.hblks, info.hblkhd, info.uordblks};
	*total_free_heap_space = info.fordblks;
	return 1;
}

static int test_main_thread(int use_mallinfo2)
{
	malloc_thread_stats_t total_stats = {0};

	malloc_thread_stats_t total_stats_before_test = {0};
	long long free_heap_space_before_test = 0;
	int result = stats_from_mallinfo(&total_stats_before_test, &free_heap_space_before_test, use_mallinfo2);

	void *ptrs[SIZES_COUNT];

	for (size_t i = 0; i < SIZES_COUNT; i++) {
		ptrs[i] = malloc(sizes[i]);
	}
	long long free_heap_space_after_allocations = 0;
	result &= stats_from_mallinfo(&total_stats, &free_heap_space_after_allocations, use_mallinfo2);
	result &= validate_total_allocated(&total_stats);
	for (size_t i = 0; i < SIZES_COUNT; i++) {
		free(ptrs[i]);
	}
	long long free_heap_space_after_free = 0;
	result &= stats_from_mallinfo(&total_stats, &free_heap_space_after_free, use_mallinfo2);
	total_stats.mmapped_regions -= total_stats_before_test.mmapped_regions;
	total_stats.total_allocated_memory -= total_stats_before_test.total_allocated_memory;
	total_stats.total_mmapped_memory -= total_stats_before_test.total_mmapped_memory;
	result &= validate_all_freed(&total_stats);
	result &= expect_greater_equal(
		free_heap_space_after_free,
		free_heap_space_after_allocations,
		"free heap space after free",
		"free heap space after allocations");
	return result;
}

static int test_different_threads(int use_mallinfo2)
{
	malloc_thread_stats_t total_stats = {0};
	malloc_thread_stats_t total_stats_before_test = {0};
	long long free_heap_space_before_test = 0;

	int result = stats_from_mallinfo(&total_stats_before_test, &free_heap_space_before_test, use_mallinfo2);

	pthread_barrier_t alloc_barrier, free_barrier;
	if (pthread_barrier_init(&alloc_barrier, NULL, SIZES_COUNT + 1)) {
		return 0;
	}
	if (pthread_barrier_init(&free_barrier, NULL, SIZES_COUNT + 1)) {
		return 0;
	}

	thread_data_t thread_data[SIZES_COUNT];
	for (size_t i = 0; i < SIZES_COUNT; i++) {
		thread_data[i] = (thread_data_t) {sizes[i], &alloc_barrier, &free_barrier, 0};
	}
	pthread_t threads[SIZES_COUNT];
	for (size_t i = 0; i < SIZES_COUNT; i++) {
		pthread_create(&threads[i], NULL, allocate_wait_free, &thread_data[i]);
	}
	pthread_barrier_wait(&alloc_barrier);
	long long free_heap_space_after_allocations = 0;
	result &= stats_from_mallinfo(&total_stats, &free_heap_space_after_allocations, use_mallinfo2);
	result &= validate_total_allocated(&total_stats);

	pthread_barrier_wait(&free_barrier);
	for (size_t i = 0; i < SIZES_COUNT; i++) {
		pthread_join(threads[i], NULL);
	}
	long long free_heap_space_after_free = 0;
	result &= stats_from_mallinfo(&total_stats, &free_heap_space_after_free, use_mallinfo2);
	total_stats.mmapped_regions -= total_stats_before_test.mmapped_regions;
	total_stats.total_allocated_memory -= total_stats_before_test.total_allocated_memory;
	total_stats.total_mmapped_memory -= total_stats_before_test.total_mmapped_memory;
	result &= validate_all_freed(&total_stats);
	result &= expect_greater_equal(
		free_heap_space_after_free,
		free_heap_space_after_allocations,
		"free heap space after free",
		"free heap space after allocations");
	return result;
}

static int test_and_report(
	int (*test_func)(int),
	int test_func_arg,
	const char *message)
{
	t_printf("%s...", message);
	if (!test_func(test_func_arg)) {
		t_error("Failed!\n");
		return 0;
	}
	t_printf("Success\n");
	return 1;
}

int main(void)
{
	int result = test_and_report(test_main_thread, 0, "Testing mallinfo main thread");
	result &= test_and_report(test_main_thread, 1, "Testing mallinfo2 main thread");
	result &= test_and_report(test_different_threads, 0, "Testing mallinfo different threads");
	result &= test_and_report(test_different_threads, 1, "Testing mallinfo2 different threads");
	return result == 0;
}