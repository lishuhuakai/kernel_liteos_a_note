/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include "pthread_impl.h"

int __pthread_rwlock_timedwrlock(pthread_rwlock_t *restrict rw, const struct timespec *restrict at)
{
    if (rw == NULL) {
        return EINVAL;
    }
    int r, t;
    int clock = (rw->_rw_clock == CLOCK_MONOTONIC) ? CLOCK_MONOTONIC : CLOCK_REALTIME;

    r = pthread_rwlock_trywrlock(rw);
    if (r != EBUSY) return r;

    int spins = 100;
    while (spins-- && rw->_rw_lock && !rw->_rw_waiters) a_spin();

    while ((r=__pthread_rwlock_trywrlock(rw))==EBUSY) {
        if (!(r=rw->_rw_lock)) continue;
        t = r | 0x80000000;
        a_inc(&rw->_rw_waiters);
        a_cas(&rw->_rw_lock, r, t);
        r = __timedwait(&rw->_rw_lock, t, clock, at, rw->_rw_shared^128);
        a_dec(&rw->_rw_waiters);
        if (r && r != EINTR) return r;
    }
    return r;
}

weak_alias(__pthread_rwlock_timedwrlock, pthread_rwlock_timedwrlock);
