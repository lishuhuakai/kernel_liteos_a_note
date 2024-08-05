#include <stdint.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

/* TODO: not thread-safe nor fork-safe */
extern volatile int t_status;

#define T_LOC2(l) __FILE__ ":" #l
#define T_LOC1(l) T_LOC2(l)
#define t_error(...) t_printf(T_LOC1(__LINE__) ": " __VA_ARGS__)

int t_printf(const char *s, ...);

int t_vmfill(void **, size_t *, int);
int t_memfill(void);

void t_fdfill(void);

void t_randseed(uint64_t s);
uint64_t t_randn(uint64_t n);
uint64_t t_randint(uint64_t a, uint64_t b);
void t_shuffle(uint64_t *p, size_t n);
void t_randrange(uint64_t *p, size_t n);
int t_choose(uint64_t n, size_t k, uint64_t *p);

char *t_pathrel(char *buf, size_t n, char *argv0, char *p);

int t_setrlim(int r, long lim);

int t_setutf8(void);

void check_log(const char *file, const char *pattern);
void clear_log(const char *log_dir, const char *file_tag);
// Use the file_tag to match all the files in log_dir, and then check if the file contain pattern.
void find_and_check_file(const char *log_dir, const char *file_tag, const char *pattern);

#ifndef __arm__
#define __clock_gettime64 clock_gettime
#define __pthread_cond_timedwait_time64 pthread_cond_timedwait
#define  __nanosleep_time64 nanosleep
#define __fstat_time64 fstat
#define __lstat_time64 lstat
#define __stat_time64 stat
#define __utimensat_time64 utimensat
#define __adjtimex_time64 adjtimex
#define __clock_adjtime64 clock_adjtime
#define __clock_getres_time64 clock_getres
#define __clock_nanosleep_time64 clock_nanosleep
#define __clock_settime64 clock_settime
#define __cnd_timedwait_time64 cnd_timedwait
#define __ctime64 ctime
#define __ctime64_r ctime_r
#define __difftime64 difftime
#define __dlsym_time64 dlsym
#define __fstatat_time64 fstatat
#define __futimens_time64 futimens
#define __futimes_time64 futimes
#define __futimesat_time64 futimesat
#define __getitimer_time64 getitimer
#define __getrusage_time64 getrusage
#define __gettimeofday_time64 gettimeofday
#define __gmtime64 gmtime
#define __gmtime64_r gmtime_r
#define __localtime64 localtime
#define __localtime64_r localtime_r
#define __lutimes_time64 lutimes
#define __mktime64 mktime
#define __mtx_timedlock_time64 mtx_timedlock
#define __ppoll_time64 ppoll
#define __pselect_time64 pselect
#define __pthread_mutex_timedlock_time64 pthread_mutex_timedlock
#define __pthread_rwlock_timedrdlock_time64 pthread_rwlock_timedrdlock
#define __pthread_rwlock_timedwrlock_time64 pthread_rwlock_timedwrlock
#define __recvmmsg_time64 recvmmsg
#define __sched_rr_get_interval_time64 sched_rr_get_interval
#define __select_time64 select
#define __sem_timedwait_time64 sem_timedwait
#define __semtimedop_time64 semtimedop
#define __setitimer_time64 setitimer
#define __settimeofday_time64 settimeofday
#define __sigtimedwait_time64 sigtimedwait
#define __thrd_sleep_time64 thrd_sleep
#define __time64 time
#define __timegm_time64 timegm
#define __timer_gettime64 timer_gettime
#define __timer_settime64 timer_settime
#define __timerfd_gettime64 timerfd_gettime
#define __timerfd_settime64 timerfd_settime
#define __timespec_get_time64 timespec_get
#define __utime64 utime
#define __utimes_time64 utimes
#define __wait4_time64 wait4
#endif

#define TEST_MODE 0666

#ifdef __cplusplus
}
#endif
