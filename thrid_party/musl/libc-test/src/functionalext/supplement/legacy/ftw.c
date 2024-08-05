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

#include <ftw.h>
#include <sys/stat.h>
#include <unistd.h>
#include "functionalext.h"

const int FAIL = -1;
int fn(const char *file, const struct stat *sb, int flag)
{
    return 0;
}

/**
 * @tc.name      : ftw_0100
 * @tc.desc      : Each parameter is valid and can traverse the directory tree.
 * @tc.level     : Level 0
 */
void ftw_0100()
{
    int ret = -1;
    ret = ftw("/data", fn, 500);
    EXPECT_NE("ftw_0100", ret, FAIL);
}

int main(int argc, char *argv[])
{
    ftw_0100();

    return t_status;
}