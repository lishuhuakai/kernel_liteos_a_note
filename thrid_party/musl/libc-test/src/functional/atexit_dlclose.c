/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include "test.h"

typedef void (* ATEXIT_CB)();

#define TEST_DSO "/data/tests/libc-test/src/libatexit_dlclose_dso.so"
#define ATEXIT_CB_NAME "atexit_cb"
#define ATEXIT_WATCHPOINT_NAME "g_watchpoint"
#define LIBATEXIT_DLCLOSE_DSO_ABSOLUTE_PATH "/data/tests/libc-test/src/libatexit_dlclose_dso.so"

int fork_main(char *exe)
{
	char buf[512];
	void *handler = NULL;
	ATEXIT_CB cb = NULL;
	unsigned int *wp_ptr = NULL;
	unsigned int wp = 0;

	int err = 0;

	if(!t_pathrel(buf, sizeof(buf), exe, TEST_DSO)) {
		t_error("failed to obtain relative path to " TEST_DSO "\n");
		return 1;
	}

	handler = dlopen(LIBATEXIT_DLCLOSE_DSO_ABSOLUTE_PATH, RTLD_LAZY|RTLD_LOCAL);
	if(!handler) {
		t_error("dlopen %s failed: %s\n", buf, dlerror());
		return 2;
	}

	cb = (ATEXIT_CB)dlsym(handler, ATEXIT_CB_NAME);
	if (!cb) {
		t_error("dlsym %s failed: %s\n", ATEXIT_CB_NAME, dlerror());
		return 3;
	}

	wp_ptr = (unsigned int *)dlsym(handler, ATEXIT_WATCHPOINT_NAME);
	if (!wp_ptr) {
		t_error("dlsym %s failed: %s\n", ATEXIT_WATCHPOINT_NAME, dlerror());
		return 3;
	}

	wp_ptr = &wp;

	err = atexit(cb);

	if(dlclose( handler)) {
		t_error("dlclose failed: %s\n", dlerror());
		return 4;
	}

	if (wp == 0xFFFF) {
		t_error("error, atexit callback called");
		return 5;
	}

	return 0;
}

int main(int argc, char *argv[])
{
	pid_t pid, w;
	int err;
	int wstatus;

	pid = fork();
	if (pid == 0) { // child process
		return fork_main(argv[0]);
	} else if (pid > 0) { // parent process
		w = waitpid(pid, &wstatus, 0);
		if (w == -1) {
			t_error("wait for child process failed");
			return 1;
		}

		if (WIFEXITED(wstatus)) {
			err = WEXITSTATUS(wstatus);
			t_error("exited with status=%d\n", err);
			return err;
		} else if (WIFSIGNALED(wstatus)) {
			t_error("killed by signal %d\n", WTERMSIG(wstatus));
			return 9;
		} else if (WIFSTOPPED(wstatus)) {
			t_error("stopped by signal %d\n", WSTOPSIG(wstatus));
			return 9;
		} else {
			t_error("stopped by signal %d\n", WSTOPSIG(wstatus));
			return 9;
		}
	} else {
		t_error("fork failed: %s\n", strerror(errno));
		return 1;
	}
}

