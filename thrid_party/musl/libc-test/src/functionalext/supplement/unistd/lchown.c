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

#include <unistd.h>
#include "functionalext.h"
#include "filepath_util.h"

#define TEST_ID_VALUE 100

/**
 * @tc.name      : lchown_0100
 * @tc.desc      : Change ownership of a file
 * @tc.level     : Level 0
 */
void lchown_0100(void)
{
    char pathname[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH("lchown", pathname);
    int ret = lchown(pathname, TEST_ID_VALUE, TEST_ID_VALUE);
    EXPECT_EQ("lchown_0100", ret, CMPFLAG);
}

int main(void)
{
    lchown_0100();
    return t_status;
}