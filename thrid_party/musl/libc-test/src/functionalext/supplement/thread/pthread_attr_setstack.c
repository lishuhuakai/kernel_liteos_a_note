/**
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <limits.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include "functionalext.h"

#define THREAD_STACK_LEN 0x4001
#define BUFFEIZE 0x3000

/**
 * @tc.name      : pthread_attr_setstack_0100
 * @tc.desc      : Create a stack memory size of 4k, get the size value through the
 *                 pthread_attr_getstack function, whether the setting is successful
 * @tc.level     : Level 0
 */
void pthread_attr_setstack_0100(void)
{
    pthread_t thread;
    pthread_attr_t attr;
    void *stackAddr = NULL;
    void *getstackaddr = NULL;

    size_t getstackSize = 0;
    int ret = 0;
    void *p = NULL;

    int pageSize = getpagesize();
    if (pageSize == 0) {
        t_error("page Size is 0\n");
        return;
    }
    pthread_attr_init(&attr);
    ret = posix_memalign(&stackAddr, pageSize, THREAD_STACK_LEN);
    if (ret != 0) {
        EXPECT_EQ("pthread_attr_setstack_0100", ret, 0);
        return;
    }
    ret = pthread_attr_setstack(&attr, stackAddr, THREAD_STACK_LEN);
    if (ret != 0) {
        EXPECT_EQ("pthread_attr_setstack_0100", ret, 0);
        return;
    }

    pthread_attr_getstack(&attr, &getstackaddr, &getstackSize);
    EXPECT_EQ("pthread_attr_setstack_0100", getstackSize, THREAD_STACK_LEN);

    pthread_attr_destroy(&attr);
    free(stackAddr);
    stackAddr = NULL;
}

/**
 * @tc.name      : pthread_attr_setstack_0200
 * @tc.desc      : Determine whether the stack maximum function returns an abnormal value
 * @tc.level     : Level 2
 */
void pthread_attr_setstack_0200(void)
{
    pthread_attr_t attr;
    void *stackAddr = NULL;
    int ret = 0;
    int MINSIZE = PTHREAD_STACK_MIN - 1;

    pthread_attr_init(&attr);

    ret = pthread_attr_setstack(&attr, stackAddr, ERREXPECT);
    EXPECT_EQ("pthread_attr_setstack_0200", ret, EINVAL);

    ret = pthread_attr_setstack(&attr, stackAddr, MINSIZE);
    EXPECT_EQ("pthread_attr_setstack_0200", ret, EINVAL);

    pthread_attr_destroy(&attr);
}

int main(void)
{
    pthread_attr_setstack_0100();
    pthread_attr_setstack_0200();

    return t_status;
}