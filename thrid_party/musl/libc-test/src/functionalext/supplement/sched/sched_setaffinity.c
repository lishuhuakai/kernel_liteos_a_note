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

#include <stdio.h>
#include <errno.h>
#include <sched.h>
#include <unistd.h>
#include "test.h"

/*
 * @tc.name      : sched_setaffinity_0100
 * @tc.desc      : Invalid cpu set size
 * @tc.level     : Level 0
 */
void sched_setaffinity_0100(void)
{
    errno = 0;
    int max = sysconf(_SC_NPROCESSORS_CONF) + 1;
    cpu_set_t *set = CPU_ALLOC(max);
    if (set == NULL) {
        t_error("%s failed, set is NULL", __func__);
    }

    size_t set_size = CPU_ALLOC_SIZE(max);
    CPU_ZERO_S(set_size, set);
    CPU_SET_S(max, set_size, set);

    int result = sched_setaffinity(0, set_size, set);
    if (result != -1) {
        t_error("%s affinity mask does not trigger an error", __func__);
    }

    if (errno != EINVAL) {
        t_error("%s errno is %d", __func__, errno);
    }
}

/*
 * @tc.name      : sched_setaffinity_0100
 * @tc.desc      : Set cpu affinity
 * @tc.level     : Level 1
 */
void sched_setaffinity_0200(void)
{
    cpu_set_t mask;
    cpu_set_t get;

    CPU_ZERO(&mask);
    CPU_SET(0, &mask);

    int result = sched_setaffinity(0, sizeof(mask), &mask);
    if (result == -1) {
        t_error("%s failed to set cpu affinity\n", __func__);
    }

    CPU_ZERO(&get);
    result = sched_getaffinity(0, sizeof(get), &get);
    if (result == -1) {
        t_error("%s sched_getaffinity failed\n", __func__);
    }

    if (!CPU_ISSET(0, &get)) {
        t_error("%s getaffinity is not 0\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    sched_setaffinity_0100();
    sched_setaffinity_0200();
    return t_status;
}