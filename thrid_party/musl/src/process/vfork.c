#define _GNU_SOURCE
#include <unistd.h>
#include <signal.h>
#include "syscall.h"
#include "pthread_impl.h"
#ifndef __LITEOS__
#include "proc_xid_impl.h"
#endif

hidden pid_t __vfork(void)
{
	/* vfork syscall cannot be made from C code */
#ifdef SYS_fork
	return syscall(SYS_fork);
#else
	return syscall(SYS_clone, SIGCHLD, 0);
#endif
}

pid_t vfork(void)
{
	pthread_t self = __pthread_self();
	pid_t parent_pid = self->pid;
	self->pid = 0;
#ifdef __LITEOS__
	pid_t ret = __vfork();
	if (ret != 0) {
		self->pid = parent_pid;
	} else {
		self->proc_tid = -1;
	}
#else
	int parent_by_vfork = self->by_vfork;
	self->by_vfork = 1;
	pid_t ret = __vfork();
	if (ret != 0) {
		self->pid = parent_pid;
		self->by_vfork = parent_by_vfork;
	} else {
		self->proc_tid = -1;
		__clear_proc_pid();
	}
#endif
	return ret;
}
