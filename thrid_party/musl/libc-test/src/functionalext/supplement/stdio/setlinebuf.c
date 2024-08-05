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
 * @tc.name      : setlinebuf_0100
 * @tc.desc      : Set file stream to linear buffer
 * @tc.level     : Level 0
 */
void setlinebuf_0100(void)
{
    char path[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FILE_TXT, path);
    FILE *f = fopen(path, "w+");
    char buffer[BUFSIZ];
    errno = 0;
    setlinebuf(f);
    if (errno != 0) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }

    fputs(str, f);
    fclose(f);
    f = fopen(path, "r");

    memset(buffer, 0, sizeof(buffer));
    fread(buffer, sizeof(buffer), 1, f);

    if (strcmp(buffer, str)) {
        t_error("%s failed: setlinebuf. buf = %s\n", __func__, buffer);
    }
    fclose(f);
    remove(path);
}

/**
 * @tc.name      : setlinebuf_0200
 * @tc.desc      : Output to console
 * @tc.level     : Level 2
 */
void setlinebuf_0200(void)
{
    errno = 0;
    setlinebuf(stdout);
    if (errno != 0) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }

    printf("test");
    sleep(1);
}

int main(int argc, char *argv[])
{
    setlinebuf_0100();
    setlinebuf_0200();
    return t_status;
}