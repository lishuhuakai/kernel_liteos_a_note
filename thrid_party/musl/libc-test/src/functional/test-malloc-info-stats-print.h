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

#ifndef TEST_MALLOC_STATS_H
#define TEST_MALLOC_STATS_H

#define _GNU_SOURCE

#include <pthread.h>
#include <malloc.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "test-malloc-stats-common.h"
#include "test.h"

#define MAX_TID_LEN 32
#define STATS_BUFFER_SIZE 4096

typedef struct {
	char stats_before_allocations[STATS_BUFFER_SIZE];
	char stats_after_allocations[STATS_BUFFER_SIZE];
	char stats_after_free[STATS_BUFFER_SIZE];
	char threads[SIZES_COUNT][MAX_TID_LEN + 1];
} test_results_t;

static void stderr_stats_cb(void);

static int populate_thread_stats(const char *output, const char *thread_id, malloc_thread_stats_t *stats);

static int populate_total_free_heap_space(const char *output, long long *total_free_heap_space);

static int is_thread_in_output(const char *output, const char *thread_id);

static void print_to_file(void *fp, const char *s)
{
	fputs(s, fp);
}

int stats_to_buffer(char *buffer)
{
	fflush(stderr);
	int err_pipe[2];
	int saved_stderr = dup(STDERR_FILENO);
	if (pipe(err_pipe) != 0) {
		perror("Can't create pipe");
		return 0;
	}
	dup2(err_pipe[1], STDERR_FILENO);
	close(err_pipe[1]);
	stderr_stats_cb();
	fflush(stderr);
	read(err_pipe[0], buffer, STATS_BUFFER_SIZE);
	dup2(saved_stderr, STDERR_FILENO);
	return 1;
}

static test_results_t get_main_thread_test_results(void)
{
	test_results_t test_results = {{0},
	                               {0},
	                               {0},
	                               {{0}}};

	stats_to_buffer(test_results.stats_before_allocations);

	snprintf(test_results.threads[0], MAX_TID_LEN, "%d", (pid_t) syscall(__NR_gettid));

	void *ptrs[SIZES_COUNT] = {0};
	for (size_t i = 0; i < SIZES_COUNT; i++) {
		ptrs[i] = malloc(sizes[i]);
	}
	stats_to_buffer(test_results.stats_after_allocations);
	for (size_t i = 0; i < SIZES_COUNT; i++) {
		free(ptrs[i]);
	}
	stats_to_buffer(test_results.stats_after_free);
	return test_results;
}

static test_results_t get_different_threads_test_results(void)
{
	test_results_t test_results = {{0},
	                               {0},
	                               {0},
	                               {{0}}};

	stats_to_buffer(test_results.stats_before_allocations);
	pthread_barrier_t alloc_barrier, free_barrier;
	if (pthread_barrier_init(&alloc_barrier, NULL, SIZES_COUNT + 1)) {
		return test_results;
	}
	if (pthread_barrier_init(&free_barrier, NULL, SIZES_COUNT + 1)) {
		return test_results;
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

	for (size_t i = 0; i < SIZES_COUNT; i++) {
		snprintf(test_results.threads[i], MAX_TID_LEN, "%d", thread_data[i].self_id);
	}
	stats_to_buffer(test_results.stats_after_allocations);

	pthread_barrier_wait(&free_barrier);
	for (size_t i = 0; i < SIZES_COUNT; i++) {
		pthread_join(threads[i], NULL);
	}
	stats_to_buffer(test_results.stats_after_free);
	return test_results;
}

static void *allocate_and_abandon(void *arg)
{
	void **allocs = arg;
	for (size_t i = 0; i < SIZES_COUNT; i++) {
		allocs[i] = malloc(sizes[i]);
	}
	return NULL;
}

static int validate_main_thread_test_results(test_results_t *test_results)
{
	malloc_thread_stats_t stats_before_allocations = {0};
	malloc_thread_stats_t stats_after_allocations = {0};
	malloc_thread_stats_t stats_after_free = {0};
	populate_thread_stats(test_results->stats_before_allocations, test_results->threads[0], &stats_before_allocations);
	populate_thread_stats(test_results->stats_after_allocations, test_results->threads[0], &stats_after_allocations);
	populate_thread_stats(test_results->stats_after_free, test_results->threads[0], &stats_after_free);
	stats_after_free.total_mmapped_memory -= stats_before_allocations.total_mmapped_memory;
	stats_after_free.total_allocated_memory -= stats_before_allocations.total_allocated_memory;
	stats_after_free.mmapped_regions -= stats_before_allocations.mmapped_regions;
	int result = validate_total_allocated(&stats_after_allocations);
	result &= validate_all_freed(&stats_after_free);
	return result;
}

static int validate_allocated_size(size_t size, malloc_thread_stats_t *stats)
{
	int result = expect_greater_equal(stats->total_allocated_memory, size, "allocated memory", "size");
	if (size > MMAP_THRESHOLD) {
		result &= expect_greater_equal(stats->total_mmapped_memory, size, "mmapped memory", "size");
		result &= expect_equal(stats->mmapped_regions, 1, "mmapped regions");
	}
	return result;
}

static int validate_different_threads_test_results(test_results_t *test_results)
{
	int result = 1;
	for (size_t i = 0; i < SIZES_COUNT; i++) {
		malloc_thread_stats_t thread_stats = {0};
		result &= populate_thread_stats(test_results->stats_after_allocations, test_results->threads[i], &thread_stats);
		result &= validate_allocated_size(sizes[i], &thread_stats);
		if (is_thread_in_output(test_results->stats_after_free, test_results->threads[i])) {
			t_error("Thread %s did not disappear from output\n", test_results->threads[i]);
			result = 0;
		}
	}

	long long free_heap_space_after_allocations = 0;
	long long free_heap_space_after_free = 0;
	result &= populate_total_free_heap_space(test_results->stats_after_allocations, &free_heap_space_after_allocations);
	result &= populate_total_free_heap_space(test_results->stats_after_free, &free_heap_space_after_free);
	result &= expect_greater_equal(
		free_heap_space_after_free,
		free_heap_space_after_allocations,
		"free heap space after free",
		"free heap space after allocations");
	return result;
}

static int validate_and_report(
	test_results_t *test_results,
	int (*validate_test_results_func)(test_results_t *),
	const char *message)
{
	t_printf("%s...", message);
	if (!validate_test_results_func(test_results)) {
		t_error("Failed!\n");
		return 0;
	}
	t_printf("Success\n");
	return 1;
}

int main(void)
{
	test_results_t main_thread_test_results = get_main_thread_test_results();
	test_results_t different_threads_test_results = get_different_threads_test_results();
	int result = validate_and_report(
		&main_thread_test_results,
		validate_main_thread_test_results,
		"Testing allocations in main thread");
	result &= validate_and_report(
		&different_threads_test_results,
		validate_different_threads_test_results,
		"Testing allocations in different threads");
	return result == 0;
}

#endif // TEST_MALLOC_STATS_H
