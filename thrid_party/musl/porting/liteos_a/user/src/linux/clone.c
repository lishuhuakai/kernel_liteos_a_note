#define _GNU_SOURCE
#include <stdarg.h>
#include <unistd.h>
#include <sched.h>
#include <signal.h>
#include "pthread_impl.h"
#include "syscall.h"
#include "libc.h"

struct __clone_args {
	int (*func)(void *);
	void *arg;
};

static int __start_child(void *clone_args)
{
	int status;
	sigset_t set;
	int (*func)(void *) = ((struct __clone_args *)clone_args)->func;
	void *arg = ((struct __clone_args *)clone_args)->arg;

	__block_all_sigs(&set);
	pthread_t self = __pthread_self();
	self->tid = __syscall(SYS_gettid);
	self->robust_list.off = 0;
	self->robust_list.pending = 0;
	self->next = self->prev = self;
	__thread_list_lock = 0;
	libc.threads_minus_1 = 0;
	libc.exit = 0;
	signal(SIGSYS, arm_do_signal);
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
	void *mmap_stack = NULL;
	void *clone_stack = stack;
	int (*clone_func)(void *) = func;
	struct __clone_args *clone_args = (struct __clone_args *)arg;

	va_start(ap, arg);
	ptid = va_arg(ap, pid_t *);
	tls  = va_arg(ap, void *);
	ctid = va_arg(ap, pid_t *);
	va_end(ap);

	if (!(flags & (CLONE_VM | CLONE_VFORK))) {
		if (func) {
			if (!stack) {
				mmap_stack = __mmap(0, __default_stacksize, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, -1, 0);
				if (mmap_stack == MAP_FAILED) {
					errno = ENOMEM;
					return -1;
				}
				clone_args = (struct __clone_args *)((char *)mmap_stack + __default_stacksize);
			} else {
				clone_args = (struct __clone_args *)stack;
 			}
			clone_args = (struct __clone_args *)((uintptr_t)clone_args - ((uintptr_t)clone_args % sizeof(uintptr_t)));
			clone_args = (struct __clone_args *)((char *)clone_args - sizeof(struct __clone_args));
			clone_args->func = clone_func;
			clone_args->arg = arg;
			clone_func = __start_child;
			clone_stack = (void *)clone_args;
		}
	}
	ret = __syscall_ret(__clone(clone_func, clone_stack, flags, (void *)clone_args, ptid, tls, ctid));
	if (mmap_stack) {
		__munmap(mmap_stack, __default_stacksize);
	}
	return ret;
}
