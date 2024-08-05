/**
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <errno.h>
#include <sys/resource.h>
#include <sys/time.h>
#include "functionalext.h"

#define TEST_PRIORITY_MAX 20
#define TEST_PRIORITY_MIN (-20)
#define TEST_WHO_ERROR 1000

/**
 * @tc.name      : getpriority_0100
 * @tc.desc      : Get the default program scheduling priority
 * @tc.level     : Level 0
 */
void getpriority_0100(void)
{
    errno = 0;
    int ret = getpriority(PRIO_PROCESS, getpid());
    EXPECT_EQ("getpriority_0100", errno, CMPFLAG);
    EXPECT_TRUE("getpriority_0100", ret >= TEST_PRIORITY_MIN && ret <= TEST_PRIORITY_MAX);

    errno = 0;
    ret = getpriority(PRIO_PGRP, getpgid(getpid()));
    EXPECT_EQ("getpriority_0100", errno, CMPFLAG);
    EXPECT_TRUE("getpriority_0100", ret >= TEST_PRIORITY_MIN && ret <= TEST_PRIORITY_MAX);

    errno = 0;
    ret = getpriority(PRIO_USER, getuid());
    EXPECT_EQ("getpriority_0100", errno, CMPFLAG);
    EXPECT_TRUE("getpriority_0100", ret >= TEST_PRIORITY_MIN && ret <= TEST_PRIORITY_MAX);
}

/**
 * @tc.name      : getpriority_0200
 * @tc.desc      : Set and get program scheduling priority
 * @tc.level     : Level 0
 */
void getpriority_0200(void)
{
    int i, ret;
    for (i = TEST_PRIORITY_MIN; i < TEST_PRIORITY_MAX; i++) {
        errno = 0;
        ret = setpriority(PRIO_PROCESS, getpid(), i);
        EXPECT_EQ("getpriority_0200", ret, CMPFLAG);

        errno = 0;
        ret = getpriority(PRIO_PROCESS, getpid());
        EXPECT_EQ("getpriority_0200", ret, i);
        EXPECT_EQ("getpriority_0200", errno, CMPFLAG);
    }

    for (i = TEST_PRIORITY_MIN; i < TEST_PRIORITY_MAX; i++) {
        errno = 0;
        ret = setpriority(PRIO_PGRP, getpgid(getpid()), i);
        EXPECT_EQ("getpriority_0200", ret, CMPFLAG);

        errno = 0;
        ret = getpriority(PRIO_PGRP, getpgid(getpid()));
        EXPECT_EQ("getpriority_0200", ret, i);
        EXPECT_EQ("getpriority_0200", errno, CMPFLAG);
    }

    for (i = TEST_PRIORITY_MIN; i < TEST_PRIORITY_MAX; i++) {
        errno = 0;
        ret = setpriority(PRIO_USER, getuid(), i);
        EXPECT_EQ("getpriority_0200", ret, CMPFLAG);

        errno = 0;
        ret = getpriority(PRIO_USER, getuid());
        EXPECT_EQ("getpriority_0200", ret, i);
        EXPECT_EQ("getpriority_0200", errno, CMPFLAG);
    }
}

/**
 * @tc.name      : getpriority_0300
 * @tc.desc      : Provide exception parameters, get program scheduling priority
 * @tc.level     : Level 2
 */
void getpriority_0300(void)
{
    errno = 0;
    int ret = getpriority(-1, -1);
    EXPECT_EQ("getpriority_0300", ret, ERREXPECT);
    EXPECT_EQ("getpriority_0300", errno, EINVAL);

    errno = 0;
    ret = getpriority(TEST_WHO_ERROR, getpid());
    EXPECT_EQ("getpriority_0300", ret, ERREXPECT);
    EXPECT_EQ("getpriority_0300", errno, EINVAL);
}

int main(void)
{
    getpriority_0100();
    getpriority_0200();
    getpriority_0300();
    return t_status;
}