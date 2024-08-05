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

#include <errno.h>
#include <pthread.h>

#include "pthread_util.h"

#define DEFAULT_GUARD_SIZE 8192

static void pthread_guard_size_0100(void)
{
    pthread_attr_t attr = {0};
    size_t guard_size = 0;
    pthread_getattr_default_np(&attr);
    pthread_attr_getguardsize(&attr, &guard_size);
#ifdef TARGET_GUARD_SIZE
    TEST(guard_size == TARGET_GUARD_SIZE);
#else
    TEST(guard_size == DEFAULT_GUARD_SIZE);
#endif
}

TEST_FUN G_Fun_Array[] = {
    pthread_guard_size_0100,
};

int main(void)
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }
    return t_status;
}