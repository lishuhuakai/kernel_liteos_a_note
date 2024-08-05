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
 * @tc.name      : setbuffer_0100
 * @tc.desc      : The buffer type is _IOFBF
 * @tc.level     : Level 0
 */
void setbuffer_0100(void)
{
    char path[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FILE_TXT, path);
    FILE *fp = fopen(path, "w+");
    char buf[BUFSIZ] = {0};
    errno = 0;
    setbuffer(fp, buf, BUFSIZ);
    if (errno != 0) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }

    fputs(str, fp);
    fflush(fp);
    fclose(fp);

    fp = fopen(path, "r");

    memset(buf, 0, sizeof(buf));
    fread(buf, sizeof(buf), 1, fp);

    if (strcmp(buf, str)) {
        t_error("%s failed: setbuf. buf = %s\n", __func__, buf);
    }

    fclose(fp);
    remove(path);
}

/**
 * @tc.name      : setbuffer_0200
 * @tc.desc      : Stream buffering operations with NULL
 * @tc.level     : Level 1
 */
void setbuffer_0200(void)
{
    char path[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FILE_TXT, path);
    FILE *f = fopen(path, "w+");

    errno = 0;
    setbuffer(f, NULL, BUFSIZ);
    if (errno != 0) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }

    fclose(f);
    remove(path);
}

int main(int argc, char *argv[])
{
    setbuffer_0100();
    setbuffer_0200();
    return t_status;
}