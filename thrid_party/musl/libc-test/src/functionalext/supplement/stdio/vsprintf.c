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
#include "test.h"

/**
 * @tc.name      : vsprintf_0100
 * @tc.desc      : Write formatted data %d to string from variable argument list
 * @tc.level     : Level 0
 */
void vsprintf_0100(char *format, ...)
{
    char buffer[20] = {0};
    va_list aptr;
    va_start(aptr, format);
    int result = vsprintf(buffer, format, aptr);
    va_end(aptr);
    if (result < 0) {
        t_error("%s vsprintf get result is less than 0", __func__);
    }
    if (strcmp(buffer, "1")) {
        t_error("%s wrong string written to buf %s\n", __func__, buffer);
    }
}

/**
 * @tc.name      : vsprintf_0200
 * @tc.desc      : Write formatted data %f to string from variable argument list
 * @tc.level     : Level 1
 */
void vsprintf_0200(char *format, ...)
{
    char buffer[20] = {0};
    va_list aptr;
    va_start(aptr, format);
    int result = vsprintf(buffer, format, aptr);
    va_end(aptr);
    if (result < 0) {
        t_error("%s vsprintf get result is less than 0\n", __func__);
    }
    if (strncmp(buffer, "3.0", 3)) {
        t_error("%s wrong string written to buf %s\n", __func__, buffer);
    }
}

/**
 * @tc.name      : vsprintf_0300
 * @tc.desc      : Write formatted data %s to string from variable argument list
 * @tc.level     : Level 1
 */
void vsprintf_0300(char *format, ...)
{
    char buffer[20];
    va_list aptr;
    va_start(aptr, format);
    int result = vsprintf(buffer, format, aptr);
    va_end(aptr);
    if (result < 0) {
        t_error("%s vsprintf get result is less than 0\n", __func__);
    }
    if (strcmp(buffer, "vsprintf test")) {
        t_error("%s wrong string written to buf %s\n", __func__, buffer);
    }
}

int main(int argc, char *argv[])
{
    int i = 1;
    float f = 3.0f;
    char str[] = "vsprintf test";
    vsprintf_0100("%d", i);
    vsprintf_0200("%f", f);
    vsprintf_0300("%s", str);
    return t_status;
}
