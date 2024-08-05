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

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "filepath_util.h"

const int nlen = -1;
const int vlen = 5;
const char *content = "Hello world!";

#define CONTENT_LENGTH (13)

/**
 * @tc.name      : truncate_0100
 * @tc.desc      : truncate a file to a specified length
 * @tc.level     : Level 0
 */
void truncate_0100(void)
{
    char path_n[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH("files.txt", path_n);
    FILE *f = fopen(path_n, "a");
    if (f == NULL) {
        t_error("%s failed: fopen\n", __func__);
        return;
    }
    fputs(content, f);
    fclose(f);
    int result = truncate(path_n, 0);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
        return;
    }
    remove(path_n);
}

/**
 * @tc.name      : truncate_0200
 * @tc.desc      : truncate a file to a specified length
 * @tc.level     : Level 1
 */
void truncate_0200(void)
{
    char path[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FILE_TXT, path);
    FILE *f = fopen(path, "a");
    if (f == NULL) {
        t_error("%s failed: fopen\n", __func__);
        return;
    }

    fputs(content, f);
    fclose(f);

    int result = truncate(path, vlen);
    if (result != 0) {
        remove(path);

        t_error("%s failed: result = %d\n", __func__, result);
        return;
    }

    f = fopen(path, "r");
    if (f == NULL) {
        t_error("%s failed: fopen\n", __func__);
        return;
    }

    char buf[CONTENT_LENGTH] = {0};
    fgets(buf, sizeof(buf) / sizeof(char), f);
    fclose(f);

    if (strlen(buf) != vlen) {
        remove(path);

        t_error("%s failed: buf = %s\n", __func__, buf);
        return;
    }
}

/**
 * @tc.name      : truncate_0300
 * @tc.desc      : truncate a file to a negative length
 * @tc.level     : Level 2
 */
void truncate_0300(void)
{
    char path[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FILE_TXT, path);
    int result = truncate(path, nlen);
    if (result == 0) {
        t_error("%s failed: result = %d\n", __func__, result);
        return;
    }

    if (errno != EINVAL) {
        t_error("%s failed: errno = %d\n", __func__, errno);
        return;
    }
}

/**
 * @tc.name      : truncate_0400
 * @tc.desc      : truncate a directory
 * @tc.level     : Level 2
 */
void truncate_0400(void)
{
    int result = truncate("/", 0);
    if (result == 0) {
        t_error("%s failed: result = %d\n", __func__, result);
        return;
    }

    if (errno != EISDIR) {
        t_error("%s failed: errno = %d\n", __func__, errno);
        return;
    }
}

int main(int argc, char *argv[])
{
    truncate_0100();
    truncate_0200();
    truncate_0300();
    truncate_0400();

    return t_status;
}
