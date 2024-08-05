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

#include <stdio.h>
#include <pthread.h>
#include "test.h"

static pthread_t tid1;

void *threadfunc(void *arg)
{
    pthread_t result = pthread_self();
    if (tid1 != result) {
        t_error("pthread_self_0100 pthread_self get result is %ld are not want %ld", result, tid1);
    }
    return 0;
}

/**
 * @tc.name      : pthread_self_0100
 * @tc.desc      : Verify that the number of threads in the thread library is obtained successfully
 * @tc.level     : Level 0
 */
void pthread_self_0100(void)
{
    pthread_create(&tid1, NULL, threadfunc, NULL);
    pthread_join(tid1, NULL);
}

int main(int argc, char *argv[])
{
    pthread_self_0100();
    return t_status;
}