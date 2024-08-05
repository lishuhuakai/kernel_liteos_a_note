/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "pthread_impl.h"

#define PID_MAX_LEN 11
#define STATUS_LINE_SIZE 255
#define PID_STR "Tgid:"
#define TID_STR "Pid:"

static pid_t proc_pid = -1;

void __clear_proc_pid(void)
{
    proc_pid = -1;
}

static int __find_and_convert_pid(char *buf, int len, int min_len)
{
	int count = 0;
	char pid_buf[PID_MAX_LEN] = {0};
	char *str = buf;

	if (len <= min_len) {
		return -1;
	}
	while (*str != '\0') {
		if ((*str >= '0') && (*str <= '9') && (count < PID_MAX_LEN - 1)) {
			pid_buf[count] = *str;
			count++;
			str++;
			continue;
		}
		if (count > 0) {
			break;
		}
		str++;
	}
	pid_buf[count] = '\0';
	return atoi(pid_buf);
}

static int __parse_pid_form_proc(const char *path, const char *id_buf, size_t len)
{
	char buf[STATUS_LINE_SIZE] = {0};

	FILE *fp = fopen(path, "r");
	if (!fp) {
		return -errno;
	}
	while (!feof(fp)) {
		if (!fgets(buf, STATUS_LINE_SIZE, fp)) {
			fclose(fp);
			return -errno;
		}
		if (strncmp(buf, id_buf, len) == 0) {
			break;
		}
	}
	(void)fclose(fp);
	return __find_and_convert_pid(buf, strlen(buf), len);
}

pid_t getprocpid(void)
{
	int pid;

	if (proc_pid < 0) {
		pid = __parse_pid_form_proc("/proc/self/status", PID_STR, strlen(PID_STR));
		if (pid < 1) {
			return -1;
		}
		proc_pid = (pid_t)pid;
	}
	return proc_pid;
}

pid_t getproctid(void)
{
	pthread_t self = __pthread_self();
	int pid;

	if (self->proc_tid < 0) {
		pid = __parse_pid_form_proc("/proc/thread-self/status", TID_STR, strlen(TID_STR));
		if (pid < 1) {
			return -1;
		}
		self->proc_tid = (pid_t)pid;
	}
	return self->proc_tid;
}

