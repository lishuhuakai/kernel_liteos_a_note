#ifndef _PTHREAD_IMPL_H
#define _PTHREAD_IMPL_H

#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <limits.h>
#include <sys/mman.h>
#include "libc.h"
#include "syscall.h"
#include "atomic.h"
#include "futex.h"
#ifndef __LITEOS__
#include "musl_log.h"
#endif
#include "pthread_arch.h"

#define pthread __pthread
#define TLS_RESERVE_SLOT 15

struct pthread {
	/* Part 1 -- these fields may be external or
	 * internal (accessed via asm) ABI. Do not change. */
	struct pthread *self;
#ifndef TLS_ABOVE_TP
	uintptr_t *dtv;
#endif
	struct pthread *prev, *next; /* non-ABI */
	uintptr_t sysinfo;
#ifndef TLS_ABOVE_TP
#ifdef CANARY_PAD
	uintptr_t canary_pad;
#endif
	uintptr_t canary;
#endif

	/* Part 2 -- implementation details, non-ABI. */
	int tid;
	int pid;
	int proc_tid;
	int errno_val;
	int by_vfork;
	volatile int detach_state;
#ifdef FEATURE_PTHREAD_CANCEL
	volatile int cancel;
	volatile unsigned char canceldisable, cancelasync;
#endif
	unsigned char tsd_used:1;
	unsigned char dlerror_flag:1;
	unsigned char *map_base;
	size_t map_size;
	void *stack;
	size_t stack_size;
	size_t guard_size;
	void *result;
	struct __ptcb *cancelbuf;
	void **tsd;
	struct {
	    volatile void *volatile head;
	    long off;
	    volatile void *volatile pending;
	} robust_list;
	int h_errno_val;
	volatile int timer_id;
	locale_t locale;
	volatile int killlock[1];
	char *dlerror_buf;
	void *stdio_locks;
#ifdef RESERVE_SIGNAL_STACK
	void *signal_stack;
#endif

	/* musl doesn't support libc using tls, so we reserve some slots here for gwp_asan to use. */
	struct {
		uint32_t random_state;
		uint32_t next_sample_counter : 31;
		char recursive_guard : 1;
	} gwp_asan_tls;

	#ifdef CXA_THREAD_USE_TLS
	struct thread_local_dtor {
		void (*func) (void *);
		void *arg;
		void *dso_handle; // Used to located dso.
		struct thread_local_dtor* next;
	} *thread_local_dtors;
	#endif

	/* Part 3 -- the positions of these fields relative to
	 * the end of the structure is external and internal ABI. */
#ifdef TLS_ABOVE_TP
	/* The tls_slots will be accessed by kernel, so don't use it.
	 * To solve the problem that the kernel isn't synchronized with the musl,
	 * so make pre/post reserved slots for musl.
	 * pre-reserved  : tls_slots[0-4]
	 * kernel used   : tls_slots[5-9]
	 * post-reserved : tls_slot[10-14] */
	void *tls_slots[TLS_RESERVE_SLOT];
	uintptr_t canary;
	uintptr_t *dtv;
#endif
};

enum {
	DT_EXITED = 0,
	DT_EXITING,
	DT_JOINABLE,
	DT_DETACHED,
};

#define __SU (sizeof(size_t)/sizeof(int))

#define _a_stacksize __u.__s[0]
#define _a_guardsize __u.__s[1]
#define _a_stackaddr __u.__s[2]
#define _a_detach __u.__i[3*__SU+0]
#define _a_sched __u.__i[3*__SU+1]
#define _a_policy __u.__i[3*__SU+2]
#define _a_prio __u.__i[3*__SU+3]
#ifdef __LITEOS_A__
#define _a_runtime __u.__i[3*__SU+3]
#define _a_deadline __u.__i[3*__SU+4]
#define _a_period __u.__i[3*__SU+5]
#endif
#define _m_type __u.__i[0]
#define _m_lock __u.__vi[1]
#define _m_waiters __u.__vi[2]
#define _m_prev __u.__p[3]
#define _m_next __u.__p[4]
#define _m_clock __u.__i[4]
#define _m_count __u.__i[5]
#define _c_shared __u.__p[0]
#define _c_seq __u.__vi[2]
#define _c_waiters __u.__vi[3]
#define _c_clock __u.__i[4]
#define _c_lock __u.__vi[8]
#define _c_head __u.__p[1]
#define _c_tail __u.__p[5]
#define _rw_lock __u.__vi[0]
#define _rw_waiters __u.__vi[1]
#define _rw_shared __u.__i[2]
#define _rw_clock __u.__i[4]
#define _b_lock __u.__vi[0]
#define _b_waiters __u.__vi[1]
#define _b_limit __u.__i[2]
#define _b_count __u.__vi[3]
#define _b_waiters2 __u.__vi[4]
#define _b_inst __u.__p[3]

#ifndef TP_OFFSET
#define TP_OFFSET 0
#endif

#ifndef DTP_OFFSET
#define DTP_OFFSET 0
#endif

#ifdef TLS_ABOVE_TP
#define TP_ADJ(p) ((char *)(p) + sizeof(struct pthread) + TP_OFFSET)
#ifndef __LITEOS_A__
#define __pthread_self() ((pthread_t)(__get_tp() - sizeof(struct __pthread) - TP_OFFSET))
#endif
#else
#define TP_ADJ(p) (p)
#ifndef __LITEOS_A__
#define __pthread_self() ((pthread_t)__get_tp())
#endif
#endif

#ifndef tls_mod_off_t
#define tls_mod_off_t size_t
#endif

#define SIGTIMER 32
#define SIGCANCEL 33
#define SIGSYNCCALL 34

#define SIGALL_SET ((sigset_t *)(const unsigned long long [2]){ -1,-1 })
#define SIGPT_SET \
    ((sigset_t *)(const unsigned long [_NSIG/8/sizeof(long)]){ \
    [sizeof(long)==4] = 3UL<<(32*(sizeof(long)>4)) })
#define SIGTIMER_SET \
    ((sigset_t *)(const unsigned long [_NSIG/8/sizeof(long)]){ \
    0x80000000 })

void *__tls_get_addr(tls_mod_off_t *);
hidden int __init_tp(void *);
hidden void *__copy_tls(unsigned char *);
hidden void __reset_tls();

hidden void __membarrier_init(void);
hidden void __dl_thread_cleanup(void);
hidden void __testcancel();
hidden void __do_cleanup_push(struct __ptcb *);
hidden void __do_cleanup_pop(struct __ptcb *);
hidden void __pthread_tsd_run_dtors();

hidden void __pthread_key_delete_synccall(void (*)(void *), void *);
hidden int __pthread_key_delete_impl(pthread_key_t);

extern hidden volatile size_t __pthread_tsd_size;
extern hidden void *__pthread_tsd_main[];
extern hidden volatile int __eintr_valid_flag;

hidden int __clone(int (*)(void *), void *, int, void *, ...);
hidden int __set_thread_area(void *);
hidden int __libc_sigaction(int, const struct sigaction *, struct sigaction *);
hidden void __unmapself(void *, size_t);

hidden int __timedwait(volatile int *, int, clockid_t, const struct timespec *, int);
hidden int __timedwait_cp(volatile int *, int, clockid_t, const struct timespec *, int);
hidden void __wait(volatile int *, volatile int *, int, int);
static inline void __wake(volatile void *addr, int cnt, int priv)
{
    if (priv) priv = FUTEX_PRIVATE;
    if (cnt<0) cnt = INT_MAX;
    __syscall(SYS_futex, addr, FUTEX_WAKE|priv, cnt) != -ENOSYS ||
    __syscall(SYS_futex, addr, FUTEX_WAKE, cnt);
}
static inline void __futexwait(volatile void *addr, int val, int priv)
{
    if (priv) priv = FUTEX_PRIVATE;
#ifdef __LITEOS_A__
	__syscall(SYS_futex, addr, FUTEX_WAIT|priv, val, 0xffffffffu) != -ENOSYS ||
	__syscall(SYS_futex, addr, FUTEX_WAIT, val, 0xffffffffu);
#else
	__syscall(SYS_futex, addr, FUTEX_WAIT|priv, val, 0) != -ENOSYS ||
	__syscall(SYS_futex, addr, FUTEX_WAIT, val, 0);
#endif
}

#define MS_PER_S 1000
#define US_PER_S 1000000
static inline void __timespec_from_ms(struct timespec* ts, const unsigned ms)
{
    if (ts == NULL) {
        return;
    }
    ts->tv_sec = ms / MS_PER_S;
    ts->tv_nsec = (ms % MS_PER_S) * US_PER_S;
}

#define NS_PER_S 1000000000
static inline void __absolute_timespec_from_timespec(struct timespec *abs_ts,
                                                     const struct timespec *ts, clockid_t clock)
{
    if (abs_ts == NULL || ts == NULL) {
        return;
    }
    clock_gettime(clock, abs_ts);
    abs_ts->tv_sec += ts->tv_sec;
    abs_ts->tv_nsec += ts->tv_nsec;
    if (abs_ts->tv_nsec >= NS_PER_S) {
        abs_ts->tv_nsec -= NS_PER_S;
        abs_ts->tv_sec++;
    }
}

static inline int __is_mutex_destroyed(int mutex_type)
{
	return mutex_type == PTHREAD_MUTEX_DESTROYED;
}

static inline void __handle_using_destroyed_mutex(pthread_mutex_t *mutex, const char *function_name)
{
#ifndef __LITEOS__
	MUSL_LOGE("Fortify Error: %{public}s called on a destroyed mutex (%{public}p)", function_name, mutex);
#endif
}

#ifdef RESERVE_SIGNAL_STACK
hidden void pthread_reserve_signal_stack();
hidden void pthread_release_signal_stack();
#endif

hidden void __acquire_ptc(void);
hidden void __release_ptc(void);
hidden void __inhibit_ptc(void);

hidden void __tl_lock(void);
hidden void __tl_unlock(void);
hidden void __tl_sync(pthread_t);
hidden struct pthread* __pthread_list_find(pthread_t, const char*);

extern hidden volatile int __thread_list_lock;

extern hidden volatile int __abort_lock[1];

extern hidden unsigned __default_stacksize;
extern hidden unsigned __default_guardsize;

#ifdef TARGET_STACK_SIZE
#define DEFAULT_STACK_SIZE TARGET_STACK_SIZE
#else
#define DEFAULT_STACK_SIZE 131072
#endif

#ifdef TARGET_GUARD_SIZE
#define DEFAULT_GUARD_SIZE TARGET_GUARD_SIZE
#else
#define DEFAULT_GUARD_SIZE 8192
#endif

#define DEFAULT_STACK_MAX (8<<20)
#define DEFAULT_GUARD_MAX (1<<20)

#define __ATTRP_C11_THREAD ((void*)(uintptr_t)-1)

#ifdef __LITEOS_A__
#define MUSL_TYPE_THREAD    (-1)
#define MUSL_TYPE_PROCESS   (0)

#define PTHREAD_MUTEX_TYPE_MASK 3
#define PTHREAD_PRIORITY_LOWEST 31
hidden int __thread_clone(int (*func)(void *), int flags, struct pthread *thread, unsigned char *sp);
#endif

#endif
