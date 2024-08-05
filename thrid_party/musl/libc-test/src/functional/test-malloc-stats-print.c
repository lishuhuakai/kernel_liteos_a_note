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

#include <regex.h>
#include "test-malloc-info-stats-print.h"

#define THREAD_DATA_REGEX_LEN (MAX_TID_LEN + 43)
#define REGEX_NMATCH 1

static void stderr_stats_cb(void)
{
	malloc_stats_print(print_to_file, stderr, "");
}

static int parse_amount(const char **s, long long *destination)
{
	char *end_ptr = NULL;
	long long result = strtoll(*s, &end_ptr, 10);
	if (end_ptr == *s) {
		return 0;
	}
	*s = end_ptr;
	if ((!isspace(*end_ptr) && *end_ptr != '\n' && *end_ptr != '\0') || result < 0) {
		return 0;
	}
	*destination = result;
	return 1;
}

static const char *find_thread_in_output(const char *output, const char *thread_id)
{
	char thread_data_regex_s[THREAD_DATA_REGEX_LEN + 1];
	snprintf(thread_data_regex_s, THREAD_DATA_REGEX_LEN, "^%s([[:space:]]+[[:digit:]]+){3}[[:space:]]*$", thread_id);
	regex_t thread_data_regex;
	if (regcomp(&thread_data_regex, thread_data_regex_s, REG_EXTENDED | REG_NEWLINE) != 0) {
		t_error("Failed to compile regex %s", thread_data_regex_s);
		return NULL;
	}

	regmatch_t pmatch[REGEX_NMATCH];
	int match_result = regexec(&thread_data_regex, output, REGEX_NMATCH, pmatch, 0);
	regfree(&thread_data_regex);
	if (match_result != 0) {
		return NULL;
	}
	return output + pmatch[0].rm_so;
}

static int populate_thread_stats(const char *output, const char *thread_id, malloc_thread_stats_t *stats)
{
	const char *thread_data_start = find_thread_in_output(output, thread_id);
	if (thread_data_start == NULL) {
		t_error("Failed to find thread id %s in output", thread_id);
		return 0;
	}

	thread_data_start += strlen(thread_id);
	int result = 1;
	result &= parse_amount(&thread_data_start, &stats->total_allocated_memory);
	result &= parse_amount(&thread_data_start, &stats->total_mmapped_memory);
	result &= parse_amount(&thread_data_start, &stats->mmapped_regions);

	return result;
}

static int populate_total_free_heap_space(const char *output, long long *total_free_heap_space)
{
	const char *free_heap_space_start = strstr(output, "total free heap space:");
	if (free_heap_space_start == NULL) {
		return 0;
	}
	free_heap_space_start += strlen("total free heap space:");
	return parse_amount(&free_heap_space_start, total_free_heap_space);
}

static int is_thread_in_output(const char *output, const char *thread_id)
{
	return find_thread_in_output(output, thread_id) != NULL;
}