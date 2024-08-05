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

#include <limits.h>
#include <unistd.h>
#include "functionalext.h"

#define TEST_LIMIT_ERROR 1000
#define TEST_LIMIT_SIZE 3
struct limit_data {
    int name;
    long limit;
};

/**
 * @tc.name      : pathconf_0100
 * @tc.desc      : Get file-related configuration values
 * @tc.level     : Level 0
 */
void pathconf_0100(void)
{
    struct limit_data data[TEST_LIMIT_SIZE] = {
        {.name = _PC_PATH_MAX, .limit = PATH_MAX},
        {.name = _PC_PIPE_BUF, .limit = PIPE_BUF},
        {.name = _PC_NAME_MAX, .limit = NAME_MAX},
    };

    int i;
    long ret;
    for (i = 0; i < TEST_LIMIT_SIZE; i++) {
        ret = pathconf(".", data[i].name);
        EXPECT_EQ("pathconf_0100", ret, data[i].limit);
    }
}

/**
 * @tc.name      : pathconf_0200
 * @tc.desc      : Provide exception parameters to get configuration values related to files
 * @tc.level     : Level 2
 */
void pathconf_0200(void)
{
    long ret = pathconf(".", TEST_LIMIT_ERROR);
    EXPECT_EQ("pathconf_0200", ret, ERREXPECT);

    ret = pathconf(NULL, PATH_MAX);
    EXPECT_EQ("pathconf_0200", ret, ERREXPECT);

    ret = pathconf("not_exist", PATH_MAX);
    EXPECT_EQ("pathconf_0200", ret, ERREXPECT);
}

int main(void)
{
    pathconf_0100();
    pathconf_0200();
    return t_status;
}