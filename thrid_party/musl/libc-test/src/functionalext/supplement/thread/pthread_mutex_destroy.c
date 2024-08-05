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

/**
 * @tc.name      : pthread_mutex_destroy_0100
 * @tc.desc      : The parameters are valid, verify that the mutex is destroyed successfully
 * @tc.level     : Level 0
 */
void pthread_mutex_destroy_0100(void)
{
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    int result = pthread_mutex_destroy(&mutex);
    if (result != 0) {
        t_error("%s pthread_mutex_destroy error get result is %d are not want 0\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    pthread_mutex_destroy_0100();
    return t_status;
}