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

#include <time.h>
#include "functionalext.h"

extern int __nanosleep_time64(const struct timespec *, struct timespec *);

/*
 * @tc.name      : nanosleep_0100
 * @tc.desc      : Verify that the execution of the program can be paused
 * @tc.level     : Level 0
 */
void nanosleep_0100(void)
{
    struct timespec n_sleep;
    n_sleep.tv_sec = 0;
    n_sleep.tv_nsec = 5e8L;
    int data = nanosleep(&n_sleep, NULL);
    EXPECT_EQ("nanosleep_0100", data, 0);
}

/*
 * @tc.name      : nanosleep_time64_0100
 * @tc.desc      : Verify that the execution of the program can be paused with time64
 * @tc.level     : Level 0
 */
void nanosleep_time64_0100(void)
{
    struct timespec n_sleep;
    n_sleep.tv_sec = 0;
    n_sleep.tv_nsec = 5e8L;
    int data = __nanosleep_time64(&n_sleep, NULL);
    EXPECT_EQ("nanosleep_time64_0100", data, 0);
}

int main(int argc, char *argv[])
{
    nanosleep_0100();
    nanosleep_time64_0100();
    return t_status;
}