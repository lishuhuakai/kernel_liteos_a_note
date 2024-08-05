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

#include <pthread.h>
#include "test.h"

static pthread_t g_pthd_1, g_pthd_2;

void *threadfuncA(void *arg)
{
    g_pthd_1 = pthread_self();
    return arg;
}

void *threadfuncB(void *arg)
{
    pthread_t pth = pthread_self();
    int ret = pthread_equal(g_pthd_1, pth);
    if (ret != 0) {
        t_error("%s two should not equal, ret is %d\n", __func__, ret);
    }
    return arg;
}

void *threadfuncC(void *arg)
{
    int ret = pthread_equal(g_pthd_2, pthread_self());
    if (ret == 0) {
        t_error("%s the same thread, the result should be greater than zero, ret = %d\n", __func__, ret);
    }
    return arg;
}

/**
 * @tc.name      : pthread_equal_0100
 * @tc.desc      : Compare two different threads
 * @tc.level     : Level 0
 */
void pthread_equal_0100(void)
{
    pthread_t pthread1, pthread2;
    int result;

    result = pthread_create(&pthread1, NULL, threadfuncA, NULL);
    if (result != 0) {
        t_error("%s pthread_create failed\n", __func__);
        return;
    }
    result = pthread_create(&pthread2, NULL, threadfuncB, NULL);
    if (result != 0) {
        t_error("%s pthread_create failed\n", __func__);
        return;
    }

    result = pthread_join(pthread1, NULL);
    if (result != 0) {
        t_error("%s pthread_join failed, result is %d\n", __func__, result);
    }
    pthread_join(pthread2, NULL);
    if (result != 0) {
        t_error("%s pthread_join failed, result is %d\n", __func__, result);
    }
}

/**
 * @tc.name      : pthread_equal_0200
 * @tc.desc      : Compare the same thread
 * @tc.level     : Level 0
 */
void pthread_equal_0200(void)
{
    int result;

    result = pthread_create(&g_pthd_2, NULL, threadfuncC, NULL);
    if (result != 0) {
        t_error("%s pthread_create failed\n", __func__);
        return;
    }

    result = pthread_join(g_pthd_2, NULL);
    if (result != 0) {
        t_error("%s pthread_join failed, result is %d\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    pthread_equal_0100();
    pthread_equal_0200();
    return t_status;
}