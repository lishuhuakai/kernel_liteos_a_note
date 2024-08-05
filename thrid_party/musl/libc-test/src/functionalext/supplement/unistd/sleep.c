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
#include <unistd.h>
#include <time.h>
#include "test.h"

/**
 * @tc.name      : sleep_0100
 * @tc.desc      : Pause the program for a while with sleep
 * @tc.level     : Level 0
 */
void sleep_0100(void)
{
    struct timespec old_time;
    clock_gettime(CLOCK_MONOTONIC, &old_time);
    int result = sleep(2);
    struct timespec new_time;
    clock_gettime(CLOCK_MONOTONIC, &new_time);
    if (result != 0) {
        t_error("%s sleep get result is %d is not 0", __func__, result);
    }
    long int difference = new_time.tv_sec - old_time.tv_sec - 2000;
    if (difference > 2) {
        t_error("%s sleep get result is %ld is greater than 2 ms", __func__, difference);
    }
}

int main(int argc, char *argv[])
{
    sleep_0100();
    return t_status;
}