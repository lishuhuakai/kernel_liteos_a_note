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
#include <sys/stat.h>
#include "functionalext.h"
#include "filepath_util.h"

/**
 * @tc.name      : mknod_0100
 * @tc.desc      : Create file node
 * @tc.level     : Level 2
 */
void mknod_0100(void)
{
    char pathname[PATH_MAX] = "/dev/zero";
    int ret = mknod(pathname, TEST_MODE, 0);
    EXPECT_EQ("mknod_0100", ret, ERREXPECT);
}

int main(void)
{
    mknod_0100();
    return t_status;
}