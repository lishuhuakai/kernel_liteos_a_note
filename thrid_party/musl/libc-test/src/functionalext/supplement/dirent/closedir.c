/*
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

#include <dirent.h>
#include <string.h>
#include <malloc.h>
#include "functionalext.h"

#define MAXPATH 1000
#define BUFFSIZE 4096

/**
 * @tc.name      : closedir_0100
 * @tc.desc      : The parameter is valid and can close the specified directory.
 * @tc.level     : Level 0
 */
void closedir_0100(void)
{
    char path[MAXPATH];
    DIR *dir_ptr;
    char *data = NULL;
    data = getcwd(path, MAXPATH);
    dir_ptr = opendir(path);
    int ret = -1;
    ret = closedir(dir_ptr);
    EXPECT_EQ("closedir_0100", ret, 0);
}

/**
 * @tc.name      : closedir_0200
 * @tc.desc      : The dir parameter is invalid, the specified directory cannot be closed.
 * @tc.level     : Level 2
 */
void closedir_0200(void)
{
    int ret = 0;
    DIR *dir = (DIR *)malloc(BUFFSIZE);
    if (dir != NULL) {
        // fill to set dir->fd = -1 (intentionally incorrect file descriptor)
        memset(dir, 0xFF, BUFFSIZE);
        ret = closedir(dir);
    }
    EXPECT_EQ("closedir_0200", ret, -1);
}

int main(int argc, char *argv[])
{
    closedir_0100();
    closedir_0200();
    return t_status;
}