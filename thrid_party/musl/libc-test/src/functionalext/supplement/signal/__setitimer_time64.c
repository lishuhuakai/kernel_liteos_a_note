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

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "test.h"

extern int __setitimer_time64(int, const struct itimerval *__restrict, struct itimerval *__restrict);

static int count = 0;

void expireTimer(int signo)
{
    struct itimerval val;
    count++;
    if (count > 1) {
        getitimer(signo, &val);
        val.it_value.tv_sec = 0;
        val.it_value.tv_usec = 0;
        __setitimer_time64(signo, &val, NULL);
        count = 0;
    }
}

void signalHandler(int signo)
{
    switch (signo) {
        case SIGALRM:
            printf("catch a SIGALRM signal\n");
            expireTimer(signo);
            break;
        case SIGVTALRM:
            printf("catch a SIGVTALRM signal\n");
            break;
        case SIGPROF:
            printf("catch a SIGPROF signal\n");
            break;
        default:
            printf("no signal\n");
            break;
    }
}

/**
 * @tc.name      : setitimer_time64_0100
 * @tc.desc      : Set value of an interval timer
 * @tc.level     : Level 0
 */
void setitimer_time64_0100(void)
{
    signal(SIGALRM, signalHandler);
    struct itimerval it;
    memset(&it, 0, sizeof(it));

    it.it_value.tv_sec = 0;
    it.it_value.tv_usec = 100000;
    it.it_interval.tv_sec = 0;
    it.it_interval.tv_usec = 100000;

    if (__setitimer_time64(ITIMER_REAL, &it, NULL)) {
        t_error("%s set timer failed", __func__);
    }
}

int main(int argc, char *argv[])
{
    setitimer_time64_0100();
    return t_status;
}