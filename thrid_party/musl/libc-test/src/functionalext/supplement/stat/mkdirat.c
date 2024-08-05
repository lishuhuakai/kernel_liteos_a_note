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

#include <sys/stat.h>
#include "functionalext.h"

#define TEST_AT_FDCWD (-100)
#define TEST_ERROR_AT_FDCWD 100

/**
 * @tc.name      : mkdirat_0100
 * @tc.desc      : Create a subdirectory aa in the current directory
 * @tc.level     : Level 0
 */
void mkdirat_0100(void)
{
    int ret = mkdirat(TEST_AT_FDCWD, "aa", S_IRWXU | S_IRWXG | S_IXOTH | S_IROTH);
    EXPECT_EQ("mkdirat_0100", ret, CMPFLAG);

    if (access("aa", F_OK) == 0) {
        rmdir("aa");
    }
}

/**
 * @tc.name      : mkdirat_0200
 * @tc.desc      : Using the wrong fd, create a subdirectory bb in the current directory
 * @tc.level     : Level 2
 */
void mkdirat_0200(void)
{
    int ret = mkdirat(TEST_ERROR_AT_FDCWD, "bb", S_IRWXU | S_IRWXG | S_IXOTH | S_IROTH);
    EXPECT_EQ("mkdirat_0200", ret, ERREXPECT);

    if (access("bb", F_OK) == 0) {
        rmdir("bb");
    }
}

int main(void)
{
    mkdirat_0100();
    mkdirat_0200();
    return t_status;
}