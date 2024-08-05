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
#include <sys/time.h>
#include "functionalext.h"

static int count = 0;
void expireTimer(int signo)
{
    struct itimerval val;
    count++;
    if (count > 1) {
        getitimer(signo, &val);
        val.it_value.tv_sec = 0;
        val.it_value.tv_usec = 0;
        setitimer(signo, &val, NULL);
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
 * @tc.name      : getitimer_0100
 * @tc.desc      : Verify the value of the interval timer (parameter is ITIMER_REAL)
 * @tc.level     : Level 0
 */
void getitimer_0100(void)
{
    signal(SIGALRM, signalHandler);
    struct itimerval old;
    int result = getitimer(ITIMER_REAL, &old);
    EXPECT_EQ("getitimer_0100", result, 0);
}

/**
 * @tc.name      : getitimer_0200
 * @tc.desc      : Verify the value of the interval timer (parameter is ITIMER_VIRTUAL)
 * @tc.level     : Level 0
 */
void getitimer_0200(void)
{
    signal(SIGALRM, signalHandler);
    struct itimerval old;
    int result = getitimer(ITIMER_VIRTUAL, &old);
    EXPECT_EQ("getitimer_0200", result, 0);
}

/**
 * @tc.name      : getitimer_0300
 * @tc.desc      : Verify the value of the interval timer (parameter is ITIMER_PROF)
 * @tc.level     : Level 0
 */
void getitimer_0300(void)
{
    signal(SIGALRM, signalHandler);
    struct itimerval old;
    int result = getitimer(ITIMER_PROF, &old);
    EXPECT_EQ("getitimer_0300", result, 0);
}

int main(void)
{
    getitimer_0100();
    getitimer_0200();
    getitimer_0300();

    return t_status;
}