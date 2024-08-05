/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include "test.h"

#define ALIGNED_SIZE (8 * sizeof(size_t))
#define POINTER_USAGE (2 * sizeof(void *))
#define UAF_VAL 0xab
#define LOOP_SIZE 512
#define MALLOC_TIME 67

static void handler(int s)
{
}

static int child(void)
{
	char *ptr[MALLOC_TIME];
	char *ptr1[MALLOC_TIME];
	char *divide[MALLOC_TIME];
	for (int i = 0; i < LOOP_SIZE; ++i) {
		for (int j = 0; j < MALLOC_TIME; ++j) {
			ptr[j] = (char *)malloc(ALIGNED_SIZE - 1);
			if (!ptr[j]) {
				t_error("Malloc failed:%s\n", strerror(errno));
				return -1;
			}
			divide[j] = (char *)malloc(ALIGNED_SIZE - 1);
			if (!divide[j]) {
				t_error("Malloc divide failed:%s\n", strerror(errno));
				return -1;
			}
		}

		for (int j = 0; j < MALLOC_TIME; ++j) {
			free(ptr[j]);
			/* Use after free, we should avoid changing the bin/quarantine deque pointer */
			ptr[j][POINTER_USAGE] = (char)(UAF_VAL - j);
		}
		
		for (int j = 0; j < MALLOC_TIME; ++j) {
			ptr1[j] = (char *)malloc(ALIGNED_SIZE - 1);
			if (!ptr1[j]) {
				t_error("Malloc failed:%s\n", strerror(errno));
				return -1;
			}
		}

		for (int j = 0; j < MALLOC_TIME; ++j) {
			free(divide[j]);
			divide[j][POINTER_USAGE] = (char)(UAF_VAL - j);
		}

		for (int j = 0; j < MALLOC_TIME; ++j) {
			free(ptr1[j]);
			ptr1[j][POINTER_USAGE] = (char)(UAF_VAL - j);
		}
	}

	return 0;
}

static pid_t start_child(void)
{
	pid_t pid = 0;
	int ret = 0;
	pid = fork();
	if (pid == 0) {
		ret = child();
		t_error("child process normally out with %d\n", ret);
		return ret;
	}
	return pid;
}

int main(int argc, char *argv[])
{
	sigset_t set;
	int status = 0;
	pid_t pid = 0;
	int flag = 0;
	char *pname = (argc > 0) ? argv[0] : "malloc-uaf-check";

	sigemptyset(&set);
	sigaddset(&set, SIGCHLD);
	sigprocmask(SIG_BLOCK, &set, 0);
	signal(SIGCHLD, handler);

	pid = start_child();
	if (pid == -1) {
		t_error("%s fork failed: %s\n", pname, strerror(errno));
		return -1;
	}
	if (sigtimedwait(&set, 0, &(struct timespec){5, 0}) == -1) { /* Wait for 5 seconds */
		if (errno == EAGAIN)
			flag = 1;
		else
			t_error("%s sigtimedwait failed: %s\n", pname, strerror(errno));
		if (kill(pid, SIGKILL) == -1)
			t_error("%s kill failed: %s\n", pname, strerror(errno));
	}

	if (waitpid(pid, &status, 0) != pid) {
		t_error("%s waitpid failed: %s\n", pname, strerror(errno));
		return -1;
	}

	if (flag) {
		t_error("Child process time out\n");
	}

	if (WIFSIGNALED(status)) {
		if (WTERMSIG(status) != SIGSEGV && WTERMSIG(status) != SIGILL) {
			t_error("%s child process out with %s\n", pname, strsignal(WTERMSIG(status)));
			return -1;
		}
	} else {
		t_error("%s child process finished normally\n", pname);
	}
	return t_status;
}
