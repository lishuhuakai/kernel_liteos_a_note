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
#include <unistd.h>
#include "functionalext.h"
#include "filepath_util.h"

/**
 * @tc.name      : acct_0100
 * @tc.desc      : Verify logging is disabled (parameter is NULL)
 * @tc.level     : Level 2
 */
void acct_0100(void)
{
    int result = acct(NULL);
    EXPECT_EQ("acct_0200", result, -1);
}

/**
 * @tc.name      : acct_0200
 * @tc.desc      : Verify logging is disabled (parameter is file does not exist)
 * @tc.level     : Level 2
 */
void acct_0200(void)
{
    char filePath[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH("accttest.txt", filePath);
    int result = acct(filePath);
    EXPECT_EQ("acct_0300", result, -1);
    remove(filePath);
}

int main(void)
{
    acct_0100();
    acct_0200();
    return t_status;
}