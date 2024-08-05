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
#include <stdlib.h>

#include "filepath_util.h"

int filter(const struct dirent *entry)
{
    return !0;
}

/**
 * @tc.name      : scandir_0100
 * @tc.desc      : scan a directory for matching entries with alphasort
 * @tc.level     : Level 0
 */
void scandir_0100(void)
{
    struct dirent **namelist;
    char name[PATH_MAX] = {0};
    FILE_ABSOLUTE_DIR(name);

    int n = scandir(name, &namelist, NULL, alphasort);
    if (n < 0) {
        t_error("%s failed: scandir. n = %d\n", __func__, n);
        return;
    }

    while (n--) {
        free(namelist[n]);
    }
    free(namelist);
}

/**
 * @tc.name      : scandir_0200
 * @tc.desc      : scan a directory for matching entries with versionsort
 * @tc.level     : Level 1
 */
void scandir_0200(void)
{
    struct dirent **namelist;
    char name[PATH_MAX] = {0};
    FILE_ABSOLUTE_DIR(name);

    int n = scandir(name, &namelist, NULL, versionsort);
    if (n < 0) {
        t_error("%s failed: scandir. n = %d\n", __func__, n);
        return;
    }

    while (n--) {
        free(namelist[n]);
    }
    free(namelist);
}

/**
 * @tc.name      : scandir_0300
 * @tc.desc      : scan a directory for matching entries with filter
 * @tc.level     : Level 1
 */
void scandir_0300(void)
{
    struct dirent **namelist;
    char name[PATH_MAX] = {0};
    FILE_ABSOLUTE_DIR(name);
    int n = scandir(name, &namelist, filter, versionsort);
    if (n < 0) {
        t_error("%s failed: scandir. n = %d\n", __func__, n);
        return;
    }

    while (n--) {
        free(namelist[n]);
    }
    free(namelist);
}

/**
 * @tc.name      : scandir_0400
 * @tc.desc      : scan a directory for matching entries with a NULL dir
 * @tc.level     : Level 2
 */
void scandir_0400(void)
{
    struct dirent **namelist;
    int n = scandir(NULL, &namelist, NULL, alphasort);
    if (n >= 0) {
        t_error("%s failed: scandir. n = %d\n", __func__, n);

        while (n--) {
            free(namelist[n]);
        }
        free(namelist);
        return;
    }
}

int main(int argc, char *argv[])
{
    scandir_0100();
    scandir_0200();
    scandir_0300();
    scandir_0400();

    return t_status;
}
