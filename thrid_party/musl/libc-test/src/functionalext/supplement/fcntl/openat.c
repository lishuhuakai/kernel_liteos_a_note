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

#include <fcntl.h>
#include <sys/stat.h>
#include "functionalext.h"

/**
 * @tc.name      : openat_0100
 * @tc.desc      : Open /dev/null
 * @tc.level     : Level 0
 */
void openat_0100(void)
{
    int ret = openat(0, "/dev/null", O_CREAT, TEST_MODE);
    EXPECT_NE("openat_0100", ret, ERREXPECT);

    ret = openat(0, "/dev/null", O_TMPFILE, TEST_MODE);
    EXPECT_EQ("openat_0100", ret, ERREXPECT);

    ret = openat(0, "/dev/null", O_RDONLY);
    EXPECT_NE("openat_0100", ret, ERREXPECT);
}

/**
 * @tc.name      : openat_0200
 * @tc.desc      : Open file openat_test does not exist
 * @tc.level     : Level 2
 */
void openat_0200(void)
{
    int ret = openat(0, "openat_test", O_CREAT, TEST_MODE);
    EXPECT_EQ("openat_0200", ret, ERREXPECT);

    ret = openat(0, "openat_test", O_TMPFILE, TEST_MODE);
    EXPECT_EQ("openat_0200", ret, ERREXPECT);

    ret = openat(0, "openat_test", O_RDONLY);
    EXPECT_EQ("openat_0200", ret, ERREXPECT);
}

int main(void)
{
    openat_0100();
    openat_0200();
    return t_status;
}