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

#include <pwd.h>
#include "functionalext.h"

#define TEST_BUFFER_SIZE 128

/**
 * @tc.name      : getpwuid_r_0100
 * @tc.desc      : Get account information based on the current uid
 * @tc.level     : Level 0
 */
void getpwuid_r_0100(void)
{
    struct passwd pw;
    struct passwd *res = NULL;
    char buf[TEST_BUFFER_SIZE];

    int ret = getpwuid_r(getuid(), &pw, buf, sizeof(buf), &res);
    EXPECT_EQ("getpwuid_r_0100", ret, CMPFLAG);
    if (!ret) {
        EXPECT_EQ("getpwuid_r_0100", pw.pw_uid, getuid());
    }
}

/**
 * @tc.name      : getpwuid_r_0200
 * @tc.desc      : Incorrect cache size, get account information based on current uid
 * @tc.level     : Level 2
 */
void getpwuid_r_0200(void)
{
    struct passwd pw;
    struct passwd *res = NULL;
    char buf[TEST_BUFFER_SIZE];

    int ret = getpwuid_r(getuid(), &pw, buf, 1, &res);
    EXPECT_EQ("getpwuid_r_0200", ret, ERANGE);
}

int main(void)
{
    getpwuid_r_0100();
    getpwuid_r_0200();
    return t_status;
}