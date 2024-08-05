#ifndef	_ADAPT_SYS_WAIT_H
#define	_ADAPT_SYS_WAIT_H
#ifdef __cplusplus
extern "C" {
#endif

#include <sys/features.h>
#include_next <sys/wait.h>

typedef enum {
    P_ALL = 0,
    P_PID = 1,
    P_PGID = 2,
    P_PIDFD = 3
} idtype_t;

#ifdef __cplusplus
}
#endif
#endif
