/**
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

#include <langinfo.h>
#include <locale.h>
#include <stdlib.h>
#include "functionalext.h"

#define TIME_ERROR_INFO 0x20034
#define MESSAGES_ERROR_INFO 0x50004

/**
 * @tc.name      : langinfo_0100
 * @tc.desc      : Asserts whether the nl_langinfo function succeeds in reading data
 * from the MUSL_LOCPATH environment variable set to zh_CN. Utf-8(There is no file in "/etc/locale" now)
 * @tc.level     : Level 0
 */
void langinfo_0100(void)
{
    setenv("MUSL_LOCPATH", "/etc/locale", 1);
    char *lo = setlocale(LC_TIME, "");
    if (!lo) {
        EXPECT_PTRNE("nl_langinfo_0100", lo, NULL);
        return;
    }
    lo = setlocale(LC_TIME, "zh_CN.UTF-8");
    char *ptr = nl_langinfo(DAY_2);
    EXPECT_STREQ("nl_langinfo_0100", ptr, "Monday");
}

/**
 * @tc.name      : nl_langinfo_0200
 * @tc.desc      : Assert that the nl_langinfo function does not read data from the default zh_CN.UTF-8 file
 * when the locale is reset to the default environment
 * @tc.level     : Level 0
 */
void nl_langinfo_0200()
{
    char *lo = setlocale(LC_TIME, "");
    if (!lo) {
        EXPECT_PTRNE("nl_langinfo_0200", lo, NULL);
        return;
    }
    char *ptr = nl_langinfo(DAY_2);
    if (ptr) {
        EXPECT_STRNE("nl_langinfo_0200", ptr, "lm星期一");
    }
}

/**
 * @tc.name      : nl_langinfo_0300
 * @tc.desc      : Assert whether the LC_TIME data type is set to zh_CN.UTF-8 through setlocale,
 * and whether the return value is empty when the abnormal time data is passed to the nl_langinfo function.
 * MUSL_LOCPATH is `invalid` in system environment so it should be changed to `invalid` check 
 * @tc.level     : Level 2
 */
void nl_langinfo_0300()
{
    char *lo = setlocale(LC_TIME, "");
    if (!lo) {
        EXPECT_PTRNE("nl_langinfo_0300", lo, NULL);
        return;
    }
    lo = setlocale(LC_TIME, "zh_CN.UTF-8");
    char *ptr = nl_langinfo(TIME_ERROR_INFO);
    EXPECT_STREQ("nl_langinfo_0300", ptr, "");
}

/**
 * @tc.name      : nl_langinfo_0400
 * @tc.desc      : Assert whether the data type of LC_MESSAGES is set to zh_CN.UTF-8 through setlocale,
 * and whether the return value is empty when the abnormal time data is passed to the nl_langinfo function
 * MUSL_LOCPATH is `invalid` in system environment so it should be changed to `invalid` check 
 * @tc.level     : Level 2
 */
void nl_langinfo_0400()
{
    setlocale(LC_MESSAGES, "zh_CN.UTF-8");
    char *ptr = nl_langinfo(MESSAGES_ERROR_INFO);
    EXPECT_STREQ("nl_langinfo_0400", ptr, "");
}

/**
 * @tc.name      : nl_langinfo_0500
 * @tc.desc      : Assert whether the return value result is UTF-8 or ASCII
 * when the function nl_langinfo passes in the CODESET parameter
 * @tc.level     : Level 2
 */
void nl_langinfo_0500()
{
    char *lo = setlocale(LC_CTYPE, "");
    if (!lo) {
        EXPECT_PTRNE("nl_langinfo_0500", lo, NULL);
        return;
    }
    char *ptr = nl_langinfo(CODESET);
    EXPECT_STREQ("nl_langinfo_0500", ptr, "UTF-8");

    lo = setlocale(LC_TIME, "");
    if (!lo) {
        EXPECT_PTRNE("nl_langinfo_0500", lo, NULL);
        return;
    }
    ptr = nl_langinfo(CODESET);
    EXPECT_STREQ("nl_langinfo_0500", ptr, "UTF-8");
}

/**
 * @tc.name      : nl_langinfo_0600
 * @tc.desc      : Assert whether the return value result is not "C.UTF-8"
 * when the function nl_langinfo passes in the "65535" parameter
 * @tc.level     : Level 2
 */
void nl_langinfo_0600()
{
    char *lo = setlocale(LC_ALL, "");
    if (!lo) {
        EXPECT_PTRNE("nl_langinfo_0600", lo, NULL);
        return;
    }
    char *ptr = nl_langinfo(RADIXCHAR - 1);
    EXPECT_STREQ("nl_langinfo_0600", ptr, "C.UTF-8");
}

/**
 * @tc.name      : nl_langinfo_0700
 * @tc.desc      : Assert whether the return value result is not ""
 * when the function nl_langinfo passes in the THOUSEP parameter
 * @tc.level     : Level 2
 */
void nl_langinfo_0700()
{
    char *lo = setlocale(LC_ALL, "");
    if (!lo) {
        EXPECT_PTRNE("nl_langinfo_0700", lo, NULL);
        return;
    }
    char *ptr = nl_langinfo(THOUSEP);
    EXPECT_STREQ("nl_langinfo_0700", ptr, "");
}

/**
 * @tc.name      : nl_langinfo_0800
 * @tc.desc      : Assert whether the return value result is not ""
 * when the function nl_langinfo passes in the THOUSEP * LC_MONETARY parameter
 * @tc.level     : Level 2
 */
void nl_langinfo_0800()
{
    char *lo = setlocale(LC_ALL, "");
    if (!lo) {
        EXPECT_PTRNE("nl_langinfo_0800", lo, NULL);
        return;
    }
    char *ptr = nl_langinfo(THOUSEP * LC_MONETARY);
    EXPECT_STREQ("nl_langinfo_0800", ptr, "");
}

/**
 * @tc.name      : nl_langinfo_0900
 * @tc.desc      : Assert whether the return value result is not ""
 * when the function nl_langinfo passes in the RADIXCHAR * LC_MONETARY parameter
 * @tc.level     : Level 2
 */
void nl_langinfo_0900()
{
    char *lo = setlocale(LC_ALL, "");
    if (!lo) {
        EXPECT_PTRNE("nl_langinfo_0900", lo, NULL);
        return;
    }
    char *ptr = nl_langinfo(RADIXCHAR * LC_MONETARY);
    EXPECT_STREQ("nl_langinfo_0900", ptr, "");
}

int main(void)
{
    if (unsetenv("LANG") != 0) {
        perror("unsetenv failed");
        return t_status;
    }
    langinfo_0100();
    nl_langinfo_0200();
    nl_langinfo_0300();
    nl_langinfo_0400();
    nl_langinfo_0500();
    nl_langinfo_0600();
    nl_langinfo_0700();
    nl_langinfo_0800();
    nl_langinfo_0900();
    return t_status;
}