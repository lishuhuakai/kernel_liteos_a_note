/*
 * Copyright (c) 2022-2022 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _ADAPT_PTHREAD_H
#define _ADAPT_PTHREAD_H
#ifdef __cplusplus
extern "C" {
#endif

#include "los_mux_pri.h"

#ifndef POSIX_MUTEX_DEFAULT_INHERIT
#define POSIX_MUTEX_DEFAULT_INHERIT 1
#endif

typedef LosMuxAttr pthread_mutexattr_t;

typedef LosMux pthread_mutex_t;
#define pthread_mutex OsMux

typedef struct pthread_cond {
    volatile int count;      /**< The number of tasks blocked by condition */
    EVENT_CB_S event;        /**< Event object*/
    pthread_mutex_t* mutex;  /**< Mutex locker for condition variable protection */
    volatile int value;      /**< Condition variable state value*/
} pthread_cond_t;

#define pthread_exit __pthread_exit_discard
#include_next <pthread.h>
#undef pthread_exit
#undef PTHREAD_MUTEX_INITIALIZER

void pthread_exit(void *retVal);

#define PTHREAD_MUTEX_RECURSIVE_NP PTHREAD_MUTEX_RECURSIVE
#define PTHREAD_PRIO_NONE    LOS_MUX_PRIO_NONE
#define PTHREAD_PRIO_INHERIT LOS_MUX_PRIO_INHERIT
#define PTHREAD_PRIO_PROTECT LOS_MUX_PRIO_PROTECT

#if defined POSIX_MUTEX_DEFAULT_INHERIT
#define PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP \
    { OS_MUX_MAGIC, { PTHREAD_PRIO_INHERIT, OS_TASK_PRIORITY_LOWEST, PTHREAD_MUTEX_RECURSIVE_NP, 0 }, \
     { (struct LOS_DL_LIST *)NULL, (struct LOS_DL_LIST *)NULL }, \
     { (struct LOS_DL_LIST *)NULL, (struct LOS_DL_LIST *)NULL }, \
     (VOID *)NULL, 0 }

#define PTHREAD_MUTEX_INITIALIZER \
    { OS_MUX_MAGIC, { PTHREAD_PRIO_INHERIT, OS_TASK_PRIORITY_LOWEST, 0, 0 }, \
     { (struct LOS_DL_LIST *)NULL, (struct LOS_DL_LIST *)NULL }, \
     { (struct LOS_DL_LIST *)NULL, (struct LOS_DL_LIST *)NULL }, \
     (VOID *)NULL, 0 }
#elif defined POSIX_MUTEX_DEFAULT_PROTECT

#define PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP \
    { OS_MUX_MAGIC, { PTHREAD_PRIO_PROTECT, OS_TASK_PRIORITY_LOWEST, PTHREAD_MUTEX_RECURSIVE_NP, 0 }, \
     { (struct LOS_DL_LIST *)NULL, (struct LOS_DL_LIST *)NULL }, \
     { (struct LOS_DL_LIST *)NULL, (struct LOS_DL_LIST *)NULL }, \
     (VOID *)NULL, 0 }

#define PTHREAD_MUTEX_INITIALIZER \
    { OS_MUX_MAGIC, { PTHREAD_PRIO_PROTECT, OS_TASK_PRIORITY_LOWEST, 0, 0 },  \
     { (struct LOS_DL_LIST *)NULL, (struct LOS_DL_LIST *)NULL }, \
     { (struct LOS_DL_LIST *)NULL, (struct LOS_DL_LIST *)NULL }, \
     (VOID *)NULL, 0 }
#else

#define PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP \
    { OS_MUX_MAGIC, { PTHREAD_PRIO_NONE, OS_TASK_PRIORITY_LOWEST, PTHREAD_MUTEX_RECURSIVE_NP, 0 }, \
     { (struct LOS_DL_LIST *)NULL, (struct LOS_DL_LIST *)NULL }, \
     { (struct LOS_DL_LIST *)NULL, (struct LOS_DL_LIST *)NULL }, \
     (VOID *)NULL, 0 }

#define PTHREAD_MUTEX_INITIALIZER \
    { OS_MUX_MAGIC, { PTHREAD_PRIO_NONE, OS_TASK_PRIORITY_LOWEST, 0, 0 }, \
     { (struct LOS_DL_LIST *)NULL, (struct LOS_DL_LIST *)NULL }, \
     { (struct LOS_DL_LIST *)NULL, (struct LOS_DL_LIST *)NULL }, \
     (VOID *)NULL, 0 }
#endif

int pthread_attr_setaffinity_np(pthread_attr_t *, size_t, const cpu_set_t *);

#ifdef __cplusplus
}
#endif
#endif
