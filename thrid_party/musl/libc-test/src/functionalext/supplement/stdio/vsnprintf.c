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

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "test.h"
#include "functionalext.h"

void vsnprintf_test(char *str, size_t n, char *fmt, const char *func_name, ...)
{
    char s[n];
    va_list ap;
    va_start(ap, func_name);
    int result = vsnprintf(s, n, fmt, ap);
    va_end(ap);
    if (result < 0) {
        t_error("%s vsnprintf get result is %d are less 0\n", func_name, result);
    }
    if (strcmp(s, str) != 0) {
        t_error("%s vsnprintf get is '%s' are not '%s'\n", func_name, s, str);
    }
}

void vsnprintf_test2(char *fmt, const char *func_name, ...)
{
    char s[2] = "a";
    va_list ap;
    va_start(ap, func_name);
    int result = vsnprintf(s, 1, fmt, ap);
    va_end(ap);
    if (result < 0) {
        t_error("%s vsnprintf get result is %d are less 0\n", func_name, result);
    }
    if(s[0] != '\0'){
        t_error("%s vsnprintf n is 1 but s[0] is not a mull character\n", func_name);;
    }
}

void vsnprintf_zeron(char *str, char *fmt, const char *func_name, ...)
{
    char s[10];
    va_list ap;
    va_start(ap, func_name);
    int result = vsnprintf(s, 0, fmt, ap);
    va_end(ap);
    if (result < 0) {
        t_error("%s vsnprintf get result is %d are less 0\n", func_name, result);
    }
    if (strcmp(s, str) == 0) {
        t_error("%s vsnprintf get is '%s' but donnot want that\n", func_name, s);
    }
}

void vsnprintf_zeron_all(char *str, char *fmt, const char *func_name, ...)
{
    va_list ap;
    va_start(ap, func_name);
    int result = vsnprintf(0, 0, fmt, ap);
    va_end(ap);
    EXPECT_EQ(func_name, result, strlen(str));
}

int main(int argc, char *argv[])
{
    /**
     * @tc.name      : vsnprintf_0100
     * @tc.desc      : Call vsnprinf to get formatted output
     * @tc.level     : Level 0
     */
    vsnprintf_test("value is use", 13, "value is %s", "vsnprintf_0100", "use");
    /**
     * @tc.name      : vsnprintf_0200
     * @tc.desc      : The number of digits to be truncated is less than the length of the format string
     * @tc.level     : Level 1
     */
    vsnprintf_test("value is ", 10, "value is %s", "vsnprintf_0200", "use");
    /**
     * @tc.name      : vsnprintf_0300
     * @tc.desc      : The number of bits to be truncated is greater than the format string length
     * @tc.level     : Level 1
     */
    vsnprintf_test("value is use", 15, "value is %s", "vsnprintf_0300", "use");
    /**
     * @tc.name      : vsnprintf_0400
     * @tc.desc      : Truncate bits to 0
     * @tc.level     : Level 2
     */
    vsnprintf_zeron("value is use", "value is %s", "vsnprintf_0400", "use");
    /**
     * @tc.name      : vsnprintf_0500
     * @tc.desc      : truncate buffer, and bits to 0
     * @tc.level     : Level 3
     */
    vsnprintf_zeron_all("value is use", "value is %s", "vsnprintf_0500", "use");
    /**
     * @tc.name      : vsnprintf_0600
     * @tc.desc      : The number of bits to be 1
     * @tc.level     : Level 2
     */
    vsnprintf_test("", 1, "value is %s", "vsnprintf_0600", "use");
     /**
     * @tc.name      : vsnprintf_0700
     * @tc.desc      : This test case aims to validate that when vsnprintf is called with a buffer size of 1, 
     *                 and the buffer initially contains data, it properly overwrites the buffer to contain only a null character.
     * @tc.level     : Level 2
     */
    vsnprintf_test2("value is %s", "vsnprintf_0700", "use");

    return t_status;
}
