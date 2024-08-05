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

#include <unistd.h>
#include <sys/resource.h>
#include "functionalext.h"

/**
 * @tc.name      : prlimit_0100
 * @tc.desc      : Set the heap size of the process, and re-get the heap size value of the process
 * @tc.level     : Level 0
 */
void prlimit_0100(void)
{
    static const unsigned long long lim = 4;
    struct rlimit new_limit = {.rlim_cur = lim, .rlim_max = lim};

    struct rlimit old_limit = {.rlim_cur = 0, .rlim_max = 0};

    int ret = prlimit(getpid(), RLIMIT_STACK, &new_limit, NULL);
    EXPECT_EQ("prlimit_0100", ret, CMPFLAG);
    ret = prlimit(getpid(), RLIMIT_STACK, NULL, &old_limit);
    EXPECT_EQ("prlimit_0100", ret, CMPFLAG);
    EXPECT_LONGLONGEQ("prlimit_0100", old_limit.rlim_cur, lim);
    EXPECT_LONGLONGEQ("prlimit_0100", old_limit.rlim_max, lim);
}

/**
 * @tc.name      : prlimit_0200
 * @tc.desc      : An illegal parameter was passed in, setting the process limit parameter failed
 * @tc.level     : Level 2
 */
void prlimit_0200(void)
{
    static const unsigned long long lim = 4;
    struct rlimit new_limit = {.rlim_cur = lim, .rlim_max = lim};

    int ret = prlimit(getpid(), -1, &new_limit, NULL);
    EXPECT_EQ("prlimit_0200", ret, ERREXPECT);

    new_limit.rlim_cur = lim + 1;
    ret = prlimit(getpid(), RLIMIT_STACK, &new_limit, NULL);
    EXPECT_EQ("prlimit_0200", ret, ERREXPECT);
}

int main(void)
{
    prlimit_0100();
    prlimit_0200();

    return t_status;
}