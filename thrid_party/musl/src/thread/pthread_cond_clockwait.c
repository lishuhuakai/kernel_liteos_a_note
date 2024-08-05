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

int pthread_cond_clockwait(pthread_cond_t *cond, pthread_mutex_t *mutex,
                           clockid_t clock_id, const struct timespec *abs_timeout)
{
    if (cond == NULL) {
        return EINVAL;
    }
    switch (clock_id) {
        case CLOCK_MONOTONIC:
            cond->_c_clock = CLOCK_MONOTONIC;
            return __pthread_cond_timedwait(cond, mutex, abs_timeout);
        case CLOCK_REALTIME:
            cond->_c_clock = CLOCK_REALTIME;
            return __pthread_cond_timedwait(cond, mutex, abs_timeout);
        default:
            return EINVAL;
    }
}