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

pthread_t pid;
int flag = 0;

void *threadfunc(void *arg)
{
    sleep(2);
    flag++;
    return 0;
}

/**
 * @tc.name      : pthread_join_0100
 * @tc.desc      : The parameters are valid, verify that the main thread does not wait for other threads to end the
 * scene
 * @tc.level     : Level 0
 */
void pthread_join_0100(void)
{
    flag = 0;
    pthread_create(&pid, NULL, threadfunc, NULL);
    int result = pthread_join(pid, NULL);
    if (result != 0) {
        t_error("%s pthread_join error get result is %d are not want 0\n", __func__, result);
    }
    if (flag != 1) {
        t_error("%s pthread_join error get flag is %d are not want 1\n", __func__, flag);
    }
}

int main(int argc, char *argv[])
{
    pthread_join_0100();
    return t_status;
}