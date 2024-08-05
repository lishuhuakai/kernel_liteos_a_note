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
#include "functionalext.h"

static pthread_mutex_t g_mutex;

/**
 * @tc.name:      pthread_mutex_trylock_0100
 * @tc.desc:      Verify process pthread_mutex_try_lock success
 * @tc.level:     level 0
 */
void pthread_mutex_trylock_0100(void)
{
    pthread_mutex_init(&g_mutex, NULL);
    int ret = pthread_mutex_trylock(&g_mutex);
    EXPECT_EQ("pthread_mutex_lock_0100", ret, 0);
    pthread_mutex_unlock(&g_mutex);
}

/**
 * @tc.name:      pthread_mutex_trylock_0200
 * @tc.desc:      Verify process pthread_mutex_try_lock fail
 * @tc.level:     level 2
 */
void pthread_mutex_trylock_0200(void)
{
    pthread_mutex_init(&g_mutex, NULL);
    pthread_mutex_lock(&g_mutex);
    int ret = pthread_mutex_trylock(&g_mutex);
    EXPECT_NE("pthread_mutex_lock_0200", ret, 0);
    pthread_mutex_unlock(&g_mutex);
}

int main(void)
{
    pthread_mutex_trylock_0100();
    pthread_mutex_trylock_0200();
    return t_status;
}