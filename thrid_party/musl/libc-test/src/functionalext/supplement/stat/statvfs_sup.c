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
#include "test.h"

/**
 * @tc.name      : statvfs_0100
 * @tc.desc      : Test statvfs to get filesystem statistics in a given path
 * @tc.level     : Level 0
 */
void statvfs_0100(void)
{
    struct statvfs sb;
    int result = statvfs("/proc", &sb);
    if (result != 0) {
        t_error("%s statvfs error get result is %d are not want 0\n", __func__, result);
    }
}

/**
 * @tc.name      : statvfs_0200
 * @tc.desc      : Test statvfs results when incoming paths, etc. are empty
 * @tc.level     : Level 2
 */
void statvfs_0200(void)
{
    int want = -1;
    int result = statvfs(NULL, NULL);
    if (result != want) {
        t_error("%s statvfs error get result is %d are not want -1\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    statvfs_0100();
    statvfs_0200();
    return t_status;
}