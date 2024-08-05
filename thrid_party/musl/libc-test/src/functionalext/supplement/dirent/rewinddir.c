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

#include <dirent.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include "filepath_util.h"

void handler(int sig)
{
    exit(t_status);
}

/**
 * @tc.name      : rewinddir_0100
 * @tc.desc      : reset directory stream
 * @tc.level     : Level 0
 */
void rewinddir_0100(void)
{
    char name[PATH_MAX] = {0};
    FILE_ABSOLUTE_DIR(name);
    DIR *dir = opendir(name);
    if (dir == NULL) {
        t_error("%s failed: opendir. name = %s\n", __func__, name);
        return;
    }

    struct dirent *result = readdir(dir);
    if (result == NULL) {
        t_error("%s failed: readdir\n", __func__);
        return;
    }

    char *d_name = result->d_name;

    while ((result = readdir(dir)) != NULL) {
    }

    rewinddir(dir);
    result = readdir(dir);
    if (result == NULL) {
        t_error("%s failed: readdir\n", __func__);
        return;
    }

    if (strcmp(d_name, result->d_name)) {
        t_error("%s failed: strcmp. d_name = %s, result->d_name = %s\n", __func__, d_name, result->d_name);
        return;
    }

    closedir(dir);
}

/**
 * @tc.name      : rewinddir_0200
 * @tc.desc      : reset directory stream with a NULL dir
 * @tc.level     : Level 2
 */
void rewinddir_0200(void)
{
    signal(SIGSEGV, handler);

    DIR *dir = NULL;
    rewinddir(dir);
}

int main(int argc, char *argv[])
{
    rewinddir_0100();
    rewinddir_0200();

    return t_status;
}
