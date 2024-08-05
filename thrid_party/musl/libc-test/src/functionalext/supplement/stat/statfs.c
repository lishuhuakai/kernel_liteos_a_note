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

#include <stdio.h>
#include <string.h>
#include <sys/statvfs.h>
#include <sys/vfs.h>
#include "test.h"

/*
 * @tc.name      : statfs_0100
 * @tc.desc      : Test statfs to read information such as the size of the directory
 * @tc.level     : Level 0
 */
void statfs_0100(void)
{
    struct statfs sb;
    int result = statfs("/proc", &sb);
    if (result != 0) {
        t_error("%s statfs error get result is %d are not want 0\n", __func__, result);
    }
}

/*
 * @tc.name      : statfs_0200
 * @tc.desc      : Test the return value of the statfs function when a non-existing directory is passed in
 * @tc.level     : Level 1
 */
void statfs_0200(void)
{
    struct statfs sb;
    int want = -1;
    int result = statfs("/does-not-exist", &sb);
    if (result != want) {
        t_error("%s statfs error get result is %d are not want -1\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    statfs_0100();
    statfs_0200();
    return t_status;
}