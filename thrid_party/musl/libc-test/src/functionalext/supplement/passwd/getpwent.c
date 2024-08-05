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

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include "functionalext.h"
#define TEST_ERROR_UID 99999999

/**
 * @tc.name      : getpwent_0100
 * @tc.desc      : Verify getpwent process success
 * @tc.level     : Level 0
 */
void getpwent_0100(void)
{
    struct passwd *user = getpwent();
    EXPECT_PTRNE("getpwent_0100", user, NULL);
}

/**
 * @tc.name      : getpwnam_0100
 * @tc.desc      : Get account data from password file
 * @tc.level     : Level 0
 */
void getpwnam_0100(void)
{
    struct passwd *ret = getpwnam("root");
    EXPECT_PTRNE("getpwnam_0100", ret, NULL);
    if (ret) {
        EXPECT_STREQ("getpwnam_0100", ret->pw_name, "root");
        EXPECT_EQ("getpwnam_0100", ret->pw_uid, CMPFLAG);
        EXPECT_EQ("getpwnam_0100", ret->pw_gid, CMPFLAG);
    }
}

/**
 * @tc.name      : getpwnam_0200
 * @tc.desc      : Retrieve data from password file for non-existing accounts
 * @tc.level     : Level 2
 */
void getpwnam_0200(void)
{
    struct passwd *ret = getpwnam("abcdefg");
    EXPECT_PTREQ("getpwnam_0200", ret, NULL);
}

/**
 * @tc.name      : getpwuid_0100
 * @tc.desc      : Get password file entry by uid
 * @tc.level     : Level 0
 */
void getpwuid_0100(void)
{
    struct passwd *ret = getpwuid(getuid());
    EXPECT_PTRNE("getpwuid_0100", ret, NULL);
    if (ret) {
        EXPECT_EQ("getpwuid_0100", ret->pw_uid, getuid());
    }
}

/**
 * @tc.name      : getpwuid_0200
 * @tc.desc      : Provide illegal uid to get password file entry
 * @tc.level     : Level 2
 */
void getpwuid_0200(void)
{
    struct passwd *ret = getpwuid(TEST_ERROR_UID);
    EXPECT_PTREQ("getpwuid_0200", ret, NULL);
}

int main(void)
{
    getpwent_0100();
    getpwnam_0100();
    getpwnam_0200();
    getpwuid_0100();
    getpwuid_0200();
    return t_status;
}
