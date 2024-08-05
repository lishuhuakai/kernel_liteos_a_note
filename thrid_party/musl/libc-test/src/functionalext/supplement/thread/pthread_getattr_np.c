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

void *threadfuncA(void *arg)
{
    return arg;
}

/**
 * @tc.name      : pthread_getattr_np_0100
 * @tc.desc      : Verify pthread_getattr_np process success
 * @tc.level     : Level 0
 */
void pthread_getattr_np_0100(void)
{
    pthread_t ph;
    pthread_create(&ph, NULL, threadfuncA, NULL);
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    void *thread_result = NULL;
    int32_t ret = pthread_getattr_np(ph, &attr);
    pthread_attr_destroy(&attr);
    pthread_join(ph, &thread_result);
    void *stack = NULL;
    size_t stacksize;
    pthread_attr_getstack(&attr, &stack, &stacksize);
    EXPECT_PTRNE("pthread_getattr_np_0100", stack, NULL);
}

int main(void)
{
    pthread_getattr_np_0100();
    return t_status;
}
