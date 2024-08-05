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

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "test.h"

int readFile(int stream, char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int result = vdprintf(stream, fmt, ap);
    va_end(ap);
    return result;
}

/**
 * @tc.name      : vdprintf_0100
 * @tc.desc      : Test the vdprintf method to write to the file
 * @tc.level     : Level 0
 */
void vdprintf_0100(void)
{
    char *value = "asdf";
    char buffer[100] = {0};
    int fp = open("/data/temp_vdprintf.txt", O_RDWR | O_CREAT, TEST_MODE);
    if (fp < 0) {
        t_error("%s open failed", __func__);
        return;
    }
    int result = readFile(fp, "%s", value);
    if (result < 0) {
        t_error("%s vdprintf get result is %d are less 0\n", __func__, result);
    }
    close(fp);
    fp = open("/data/temp_vdprintf.txt", O_RDWR);
    read(fp, buffer, strlen(value));
    if (strcmp(buffer, "asdf") != 0) {
        t_error("%s vdprintf get is '%s' are not 'asdf'\n", __func__, buffer);
    }
    close(fp);
    unlink("/data/temp_vdprintf.txt");
}

/**
 * @tc.name      : vdprintf_0200
 * @tc.desc      : Test that vdprintf returns the result when the file descriptor is incorrect
 * @tc.level     : Level 2
 */
void vdprintf_0200(void)
{
    char *value = "asdf";
    int want = -1;
    int fp = -1;
    int result = readFile(fp, "%s", value);
    if (result != want) {
        t_error("%s vdprintf get result is %d are want -1\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    vdprintf_0100();
    vdprintf_0200();
    return t_status;
}