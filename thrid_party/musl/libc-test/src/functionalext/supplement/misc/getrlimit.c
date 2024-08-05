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

#include <sys/resource.h>
#include <stdlib.h>
#include "functionalext.h"

/**
 * @tc.name      : getrlimit_0100
 * @tc.desc      : Verify getrlimit process success
 * @tc.level     : Level 0
 */
void getrlimit_0100(void)
{
    static const long lim = 42;
    static const int r = RLIMIT_NOFILE;
    struct rlimit rl;

    rl.rlim_max = lim;
    rl.rlim_cur = lim;
    int ret = setrlimit(r, &rl);
    EXPECT_EQ("getrlimit_0100", ret, 0);
    struct rlimit retrl;
    ret = getrlimit(r, &retrl);
    EXPECT_EQ("getrlimit_0100", ret, 0);
    EXPECT_LONGLONGEQ("getrlimit_0100", retrl.rlim_max, lim);
    EXPECT_LONGLONGEQ("getrlimit_0100", retrl.rlim_cur, lim);
}

/**
 * @tc.name      : getrlimit_0200
 * @tc.desc      : Verify getrlimit process success fail bacaus param is error
 * @tc.level     : Level 2
 */
void getrlimit_0200(void)
{
    struct rlimit limit;
    int ret = getrlimit(-1, &limit);
    EXPECT_EQ("getrlimit_0200", ret, -1);
}

int main(void)
{
    getrlimit_0100();
    getrlimit_0200();
    return t_status;
}
