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

#include <sys/stat.h>
#include <unistd.h>
#include "functionalext.h"

const int SUCCESS = 0;
const int FAILED = -1;

/**
 * @tc.name      : rmdir_0100
 * @tc.desc      : The parameter is valid, and the file directory can be deleted.
 * @tc.level     : Level 0
 */
void rmdir_0100(void)
{
    char *path = "testnormal";
    if (access(path, F_OK) != 0) {
        int ret = mkdir(path, 0777);
        EXPECT_EQ("rmdir_0100", ret, SUCCESS);
        int rm = rmdir(path);
        EXPECT_EQ("rmdir_0100", rm, SUCCESS);
    } else {
        remove(path);
        int ret = mkdir(path, 0777);
        EXPECT_EQ("rmdir_0100", ret, SUCCESS);
        int rm = rmdir(path);
        EXPECT_EQ("rmdir_0100", rm, SUCCESS);
    }
    path = NULL;
}

/**
 * @tc.name      : rmdir_0200
 * @tc.desc      : The parameter is invalid, the directory cannot be deleted.
 * @tc.level     : Level 2
 */
void rmdir_0200(void)
{
    char *path = "testerror";
    if (access(path, F_OK) != 0) {
        int rm = rmdir(path);
        EXPECT_EQ("rmdir_0200", rm, FAILED);
    } else {
        int rm = rmdir(path);
        EXPECT_EQ("rmdir_0200", rm, SUCCESS);
        rm = rmdir(path);
        EXPECT_EQ("rmdir_0200", rm, FAILED);
    }
    path = NULL;
}

int main(int argc, char *argv[])
{
    rmdir_0100();
    rmdir_0200();

    return t_status;
}