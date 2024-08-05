/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "functionalext.h"

static int32_t errorParam = 3;

/**
 * @tc.name      : pthread_sigmask_0100
 * @tc.desc      : Verify pthread_sigmask process success when define shield signal is SIGUSR1
 * @tc.level     : Level 1
 */
void pthread_sigmask_0100(void)
{
    sigset_t set;
    sigaddset(&set, SIGUSR1);
    int32_t ret = pthread_sigmask(SIG_BLOCK, &set, NULL);
    EXPECT_EQ("pthread_sigmask_0100", ret, 0);
    sigset_t setOne, setEmpty;
    sigaddset(&setOne, SIGUSR2);
    sigemptyset(&setEmpty);
    ret = pthread_sigmask(SIG_BLOCK, &setOne, &setEmpty);
    EXPECT_EQ("pthread_sigmask_0100", ret, 0);
    sigismember(&set, SIGUSR1);
    EXPECT_EQ("pthread_sigmask_0100", sigismember(&set, SIGUSR1), 1);
}

/**
 * @tc.name      : pthread_sigmask_0200
 * @tc.desc      : Verify pthread_sigmask process success when undefine shield signal
 * @tc.level     : Level 1
 */
void pthread_sigmask_0200(void)
{
    sigset_t set;
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGQUIT);
    int32_t ret = pthread_sigmask(SIG_BLOCK, &set, NULL);
    EXPECT_EQ("pthread_sigmask_0200", ret, 0);
    sigset_t setEmpty;
    sigemptyset(&setEmpty);
    ret = pthread_sigmask(SIG_BLOCK, &setEmpty, NULL);
    EXPECT_EQ("pthread_sigmask_0200", ret, 0);
    ret = sigismember(&set, SIGUSR1);
    EXPECT_EQ("pthread_sigmask_0200", ret, 1);
    ret = sigismember(&set, SIGQUIT);
    EXPECT_EQ("pthread_sigmask_0200", ret, 1);
}

/**
 * @tc.name      : pthread_sigmask_0300
 * @tc.desc      : Verify pthread_sigmask process fail becaus first param is error
 * @tc.level     : Level 2
 */
void pthread_sigmask_0300(void)
{
    sigset_t set;
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGQUIT);
    int32_t ret = pthread_sigmask(SIG_BLOCK + errorParam, &set, NULL);
    EXPECT_EQ("pthread_sigmask_0300", ret, EINVAL);
}

/**
 * @tc.name      : pthread_sigmask_0400
 * @tc.desc      : Verify pthread_sigmask process success when second and third param is null
 * @tc.level     : Level 2
 */
void pthread_sigmask_0400(void)
{
    sigset_t set, setEmpty;
    sigaddset(&set, SIGUSR1);
    sigemptyset(&setEmpty);
    int32_t ret = pthread_sigmask(SIG_BLOCK, &set, NULL);
    EXPECT_EQ("pthread_setschedparam_0400", ret, 0);
    ret = pthread_sigmask(SIG_BLOCK, NULL, &setEmpty);
    EXPECT_EQ("pthread_setschedparam_0400", ret, 0);
    ret = sigismember(&set, SIGUSR1);
    EXPECT_EQ("pthread_setschedparam_0400", ret, 1);
}

int main(void)
{
    pthread_sigmask_0100();
    pthread_sigmask_0200();
    pthread_sigmask_0300();
    pthread_sigmask_0400();
    return t_status;
}