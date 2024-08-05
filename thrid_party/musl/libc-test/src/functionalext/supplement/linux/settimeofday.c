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
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include "test.h"

extern int __settimeofday_time64(const struct timeval *, const struct timezone *);

/**
 * @tc.name      : settimeofday_0100
 * @tc.desc      : Test that the function returns a value when tv points to a null pointer
 * @tc.level     : Level 0
 */
void settimeofday_0100(void)
{
    int result = settimeofday(NULL, NULL);
    if (result != 0) {
        t_error("%s settimeofday failed", __func__);
    }
}

/**
 * @tc.name      : settimeofday_0200
 * @tc.desc      : Test the return value of the function when usec in the parameter tv is 1000000ULL
 * @tc.level     : Level 1
 */
void settimeofday_0200(void)
{
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 1000000ULL;

    int result = settimeofday(&tv, NULL);
    if (result != -1) {
        t_error("%s settimeofday failed", __func__);
    }
}

/**
 * @tc.name      : settimeofday_0300
 * @tc.desc      : Set the time to 10:1 on January 11, 1970
 * @tc.level     : Level 1
 */
void settimeofday_0300(void)
{
    struct timeval tv;
    tv.tv_sec = 864000 + 36000 + 60;
    tv.tv_usec = 0;

    int result = settimeofday(&tv, NULL);
    if (result == -1) {
        t_error("%s settimeofday failed", __func__);
    }
}

/**
 * @tc.name      : settimeofday_time64_0200
 * @tc.desc      : Test the return value of the function when usec in the parameter tv is 1000000ULL
 * @tc.level     : Level 1
 */
void settimeofday_time64_0200(void)
{
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 1000000ULL;

    int result = __settimeofday_time64(&tv, NULL);
    if (result != -1) {
        t_error("%s __settimeofday_time64 failed", __func__);
    }
}

int main(int argc, char *argv[])
{
    settimeofday_0100();
    settimeofday_0200();
    settimeofday_0300();
    settimeofday_time64_0200();
    return t_status;
}