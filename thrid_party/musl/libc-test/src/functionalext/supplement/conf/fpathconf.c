/*
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

#include <limits.h>
#include <unistd.h>
#include "functionalext.h"

/**
 * @tc.name      : fpathconf_0100
 * @tc.desc      : Verify that you can query values related to file system limits and options
 *                 (each parameter is valid, the name parameter is _PC_LINK_MAX,)
 * @tc.level     : Level 0
 */
void fpathconf_0100(void)
{
    long result = fpathconf(0, _PC_LINK_MAX);
    EXPECT_EQ("fpathconf_0100", result, _POSIX_LINK_MAX);
}

/**
 * @tc.name      : fpathconf_0200
 * @tc.desc      : Verify that you can query values related to file system limits and options
 *                 (each parameter is valid, the name parameter is _PC_SOCK_MAXBUF,)
 * @tc.level     : Level 0
 */
void fpathconf_0200(void)
{
    long result = fpathconf(0, _PC_SOCK_MAXBUF);
    EXPECT_EQ("fpathconf_0200", result, -1);
}

/**
 * @tc.name      : fpathconf_0300
 * @tc.desc      : Verify that you can query values related to file system limits and options
 *                 (each parameter is valid, the name parameter is _PC_FILESIZEBITS,)
 * @tc.level     : Level 0
 */
void fpathconf_0300(void)
{
    long result = fpathconf(0, _PC_FILESIZEBITS);
    EXPECT_EQ("fpathconf_0300", result, FILESIZEBITS);
}

/**
 * @tc.name      : fpathconf_0400
 * @tc.desc      : Verify that you can query values related to file system limits and options
 *                 (each parameter is valid, the name parameter is _PC_2_SYMLINKS,)
 * @tc.level     : Level 0
 */
void fpathconf_0400(void)
{
    long result = fpathconf(0, _PC_2_SYMLINKS);
    EXPECT_EQ("fpathconf_0400", result, 1);
}

/**
 * @tc.name      : fpathconf_0500
 * @tc.desc      : Verify that you can query values related to file system limits and options
 *                  (each parameter is valid, the name parameter is 1000,)
 * @tc.level     : Level 2
 */
void fpathconf_0500(void)
{
    long result = fpathconf(0, 1000);
    EXPECT_EQ("fpathconf_0500", result, -1);
}

int main(int argc, char *argv[])
{
    fpathconf_0100();
    fpathconf_0200();
    fpathconf_0300();
    fpathconf_0400();
    fpathconf_0500();
    return t_status;
}