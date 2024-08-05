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

#include "filepath_util.h"

const char *str = "Hello";

/**
 * @tc.name      : setbuf_0100
 * @tc.desc      : stream buffering operations
 * @tc.level     : Level 0
 */
void setbuf_0100(void)
{
    char buf[BUFSIZ] = {0};
    char path[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FILE_TXT, path);
    FILE *f = fopen(path, "w+");
    errno = 0;
    setbuf(f, buf);
    if (errno != 0) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
    fputs(str, f);
    fflush(f);
    fclose(f);
    f = fopen(path, "r");
    memset(buf, 0, sizeof(buf));
    fread(buf, sizeof(buf), 1, f);
    if (strcmp(buf, str)) {
        t_error("%s failed: setbuf. buf = %s\n", __func__, buf);
    }
    fclose(f);
    remove(path);
}

/**
 * @tc.name      : setbuf_0200
 * @tc.desc      : stream buffering operations with NULL
 * @tc.level     : Level 2
 */
void setbuf_0200(void)
{
    char path[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FILE_TXT, path);
    FILE *f = fopen(path, "w+");

    errno = 0;
    setbuf(f, NULL);
    if (errno != 0) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }

    fclose(f);

    remove(path);
}

int main(int argc, char *argv[])
{
    setbuf_0100();
    setbuf_0200();

    return t_status;
}
