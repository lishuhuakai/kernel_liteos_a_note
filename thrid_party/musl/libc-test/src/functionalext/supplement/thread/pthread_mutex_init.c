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
#include <stdlib.h>
#include <unistd.h>
#include "functionalext.h"

static pthread_mutex_t g_mutex;

/**
 * @tc.name:      pthread_mutex_init_0100
 * @tc.desc:      Verify pthread_mutex_init process success when second args is null.
 * @tc.desc:      level 0
 */
void pthread_mutex_init_0100(void)
{
    int32_t ret = pthread_mutex_init(&g_mutex, NULL);
    EXPECT_EQ("pthread_mutex_init_0100", ret, 0);
}

/**
 * @tc.name:      pthread_mutex_init_0200
 * @tc.desc:      Verify pthread_mutex_init process success when second args is PTHREAD_MUTEX_RECURSIVE.
 * @tc.desc:      level 1
 */
void pthread_mutex_init_0200(void)
{
    int32_t type = PTHREAD_MUTEX_RECURSIVE;
    int32_t ret = pthread_mutex_init(&g_mutex, (pthread_mutexattr_t *)(&type));
    EXPECT_EQ("pthread_mutex_init_0200", ret, 0);
}

/**
 * @tc.name:      pthread_mutex_init_0300
 * @tc.desc:      Verify pthread_mutex_init process success when second args is PTHREAD_MUTEX_RECURSIVE.
 * @tc.desc:      level 1
 */
void pthread_mutex_init_0300(void)
{
    int32_t type = PTHREAD_MUTEX_ERRORCHECK;
    int32_t ret = pthread_mutex_init(&g_mutex, (pthread_mutexattr_t *)(&type));
    EXPECT_EQ("pthread_mutex_init_0300", ret, 0);
}

int main(void)
{
    pthread_mutex_init_0100();
    pthread_mutex_init_0200();
    pthread_mutex_init_0300();
    return t_status;
}
