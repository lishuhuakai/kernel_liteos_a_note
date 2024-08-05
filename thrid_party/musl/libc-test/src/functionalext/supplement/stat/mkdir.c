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
#include <sys/stat.h>
#include "functionalext.h"

const int SUCCESS = 0;
const int FAILED = -1;

/**
 * @tc.name      : mkdir_0100
 * @tc.desc      : The parameters are valid and the directoty can be created.
 * @tc.level     : Level 0
 */
void mkdir_0100(void)
{
    char *path = "mkdirtest";
    if (access(path, F_OK) != 0) {
        int ret = mkdir(path, 0777);
        EXPECT_EQ("mkdir_0100", ret, SUCCESS);
        remove(path);
    } else {
        remove(path);
        int ret = mkdir(path, 0777);
        EXPECT_EQ("mkdir_0100", ret, SUCCESS);
        remove(path);
    }
    path = NULL;
}

/**
 * @tc.name      : mkdir_0200
 * @tc.desc      : The path parameters is invalid,the directoty cannot be created.
 * @tc.level     : Level 2
 */
void mkdir_0200(void)
{
    char *path = "mkdirtest";
    if (access(path, F_OK) == 0) {
        int ret = mkdir(path, 0777);
        EXPECT_EQ("mkdir_0200", ret, FAILED);
        remove(path);
    } else {
        int retcreate = mkdir(path, 0777);
        EXPECT_EQ("mkdir_0200", retcreate, SUCCESS);
        int ret = mkdir(path, 0777);
        EXPECT_EQ("mkdir_0200", ret, FAILED);
        remove(path);
    }
    path = NULL;
}

int main(int argc, char *argv[])
{
    mkdir_0100();
    mkdir_0200();

    return t_status;
}