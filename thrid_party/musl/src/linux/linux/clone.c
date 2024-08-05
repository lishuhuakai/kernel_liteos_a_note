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

#define _GNU_SOURCE
#include <stdarg.h>
#include <unistd.h>
#include <sched.h>
#include <signal.h>
#include "pthread_impl.h"
#include "syscall.h"
#include "libc.h"
#include "proc_xid_impl.h"

struct __clone_args {
	int (*func)(void *);
	void *arg;
};

#if defined(__arm__) || defined(__aarch64__)
#define GET_SP_REG(stack)	__asm__ __volatile__ ("mov %0, sp" : "=r"(stack))
#elif defined(__x86_64__)
#define GET_SP_REG(stack)	do { \
	__asm__ __volatile__ ("mov %%rsp, %0;" : "=r"(stack) :); \
	stack = (void *)((uintptr_t)stack - 16); \
} while (0)
#else
#define GET_SP_REG(stack)
#endif

static int __start_child(void *clone_args)
{
	int status;
	sigset_t set;
	int (*func)(void *) = ((struct __clone_args *)clone_args)->func;
	void *arg = ((struct __clone_args *)clone_args)->arg;
	free(clone_args);

	__block_all_sigs(&set);
	pthread_t self = __pthread_self();
	self->tid = __syscall(SYS_gettid);
	self->pid = self->tid;
	self->proc_tid = -1;
	self->robust_list.off = 0;
	self->robust_list.pending = 0;
	self->next = self->prev = self;
	__thread_list_lock = 0;
	libc.threads_minus_1 = 0;
	__clear_proc_pid();
	__restore_sigs(&set);

	status = func(arg);
	exit(status);
}

int clone(int (*func)(void *), void *stack, int flags, void *arg, ...)
{
	int ret;
	va_list ap;
	pid_t *ptid, *ctid;
	void  *tls;
	int (*clone_func)(void *) = func;
	struct __clone_args *clone_args = (struct __clone_args *)arg;

	va_start(ap, arg);
	ptid = va_arg(ap, pid_t *);
	tls  = va_arg(ap, void *);
	ctid = va_arg(ap, pid_t *);
	va_end(ap);

	pthread_t self = __pthread_self();
	pid_t parent_pid = self->pid;
	self->pid = 0;
	pid_t caller_tid = self->tid;

	if (!(flags & (CLONE_VM | CLONE_VFORK))) {
		self->tid = -1;
		if (func) {
			clone_args = (struct __clone_args *)malloc(sizeof(struct __clone_args));
			if (clone_args == NULL) {
				errno = ENOMEM;
				return -1;
			}
			clone_args->func = clone_func;
			clone_args->arg = arg;
			clone_func = __start_child;
		}
	}
	if (!stack) {
		GET_SP_REG(stack);
	}
	ret = __syscall_ret(__clone(clone_func, stack, flags, (void *)clone_args, ptid, tls, ctid));
	if (!(flags & (CLONE_VM | CLONE_VFORK)) && func) {
		free(clone_args);
	}

	if (ret != 0) {
		self->pid = parent_pid;
		self->tid = caller_tid;
	} else if (self->tid == -1) {
		self->tid = __syscall(SYS_gettid);
		self->pid = self->tid;
	}
	return ret;
}
