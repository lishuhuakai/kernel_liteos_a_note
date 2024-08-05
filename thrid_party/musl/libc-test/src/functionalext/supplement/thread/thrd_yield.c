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

#include <time.h>
#include <threads.h>
#include "test.h"

#define ms (1000.0)
#define us (1000000)
#define ns (1000000000L)

double ustimer(struct timespec tss, struct timespec tse)
{
    double sd = difftime(tse.tv_sec, tss.tv_sec);
    long nsd = tse.tv_nsec - tss.tv_nsec;
    if (nsd < 0) {
        return us * (sd - 1) + (ns + nsd) / ms;
    } else {
        return us * (sd) + nsd / ms;
    }
}

void yieldfunc(void)
{
    struct timespec tss, tse;
    timespec_get(&tss, TIME_UTC);
    do {
        thrd_yield();
        timespec_get(&tse, TIME_UTC);
    } while (ustimer(tss, tse) < 100.0);
}

/**
 * @tc.name      : thrd_yield_0100
 * @tc.desc      : Busy wait while yielding
 * @tc.level     : Level 0
 */
void thrd_yield_0100(void)
{
    int result;
    struct timespec tss, tse;

    result = timespec_get(&tss, TIME_UTC);
    if (result != TIME_UTC) {
        t_error("%s timespec_get start failed", __func__);
        return;
    }

    yieldfunc();

    result = timespec_get(&tse, TIME_UTC);
    if (result != TIME_UTC) {
        t_error("%s timespec_get end failed", __func__);
        return;
    }

    if ((ustimer(tss, tse) - 100.0) < 0) {
        t_error("%s sleep time is not 100us", __func__);
        return;
    }
}

int main(int argc, char *argv[])
{
    thrd_yield_0100();
    return t_status;
}