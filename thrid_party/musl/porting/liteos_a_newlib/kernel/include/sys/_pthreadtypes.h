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

#ifndef _ADAPT_SYS_PTHREADTYPES_H
#define _ADAPT_SYS_PTHREADTYPES_H

#define pthread_attr_t __pthread_attr_t_discard
#define pthread_mutex_t __pthread_mutex_t_discard
#define pthread_mutexattr_t __pthread_mutexattr_t_discard
#define pthread_cond_t __pthread_cond_t_discard
#define pthread_condattr_t __pthread_condattr_t_discard
#define pthread_once_t __pthread_once_t_discard
#define pthread_barrierattr_t __pthread_barrierattr_t_discard

#include_next <sys/_pthreadtypes.h>

#undef pthread_attr_t
#undef pthread_mutex_t
#undef pthread_mutexattr_t
#undef pthread_cond_t
#undef pthread_condattr_t
#undef pthread_once_t
#undef pthread_barrierattr_t

#include "los_config.h"

#define CPU_SETSIZE 32
typedef struct cpu_set_t { unsigned long __bits[CPU_SETSIZE/(8 * sizeof(unsigned long int))]; } cpu_set_t;

typedef struct {
    unsigned int detachstate;
    unsigned int schedpolicy;
    struct sched_param schedparam;
    unsigned int inheritsched;
    unsigned int scope;
    unsigned int stackaddr_set;
    void *stackaddr;
    unsigned int stacksize_set;
    size_t stacksize;
#ifdef LOSCFG_KERNEL_SMP
    cpu_set_t cpuset;
#endif
} pthread_attr_t;

typedef int pthread_once_t;
typedef struct { int clock; } pthread_condattr_t;
typedef struct { unsigned __attr; } pthread_barrierattr_t;

#define PTHREAD_KEYS_MAX 128
#define PTHREAD_STACK_MIN LOS_TASK_MIN_STACK_SIZE
#define PTHREAD_DESTRUCTOR_ITERATIONS 4
#define PTHREAD_ATTR_STACKSIZE LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE
#define PTHREAD_PRIORITY_SCHEDULING SCHED_OTHER
#define PTHREAD_THREADS_MAX (LOSCFG_BASE_CORE_TSK_LIMIT + 1)
#define PTHREAD_MUTEXATTR_INITIALIZER { PTHREAD_MUTEX_RECURSIVE }

#endif /* !_ADAPT_SYS_PTHREADTYPES_H */
