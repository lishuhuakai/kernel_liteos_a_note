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

#ifndef _ADAPT_SYS_TYPES_H
#define _ADAPT_SYS_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif
#include "los_config.h"
#include_next <sys/types.h>

#define LOGIN_NAME_MAX 256
#define DELAYTIMER_MAX 32
#define SYS_CLK_TCK _POSIX_CLK_TICK
#define SIGQUEUE_MAX 32
#define SSIZE_MAX LONG_MAX
#define STREAM_MAX 8
#define SEM_NSEMS_MAX LOSCFG_BASE_IPC_SEM_LIMIT
#define SEM_VALUE_MAX OS_SEM_COUNT_MAX
#define RTSIG_MAX 8
#define TIMERS LOSCFG_BASE_CORE_SWTMR
#define MQ_OPEN_MAX _LITEOS_MQ_OPEN_MAX
#define TTY_NAME_MAX 32
#define TZNAME_MAX 6
#define TIMER_MAX _POSIX_TIMER_MAX
#define GETPW_R_SIZE_MAX _POSIX_GETPW_R_SIZE_MAX
#define GETGR_R_SIZE_MAX _POSIX_GETGR_R_SIZE_MAX
#define _LITEOS_MQ_OPEN_MAX LOSCFG_BASE_IPC_QUEUE_LIMIT
#define roundup(n,d) (howmany(n,d)*(d))

typedef off_t off64_t;
typedef unsigned short sa_family_t;
typedef unsigned socklen_t;
struct iovec { void *iov_base; size_t iov_len; };

#ifdef __cplusplus
}
#endif

#endif
