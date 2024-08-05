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

#include <stdlib.h>
#include <time.h>
#include "asctime_data.h"
#include "functionalext.h"
#include "strftime_data.h"

static time_t gTime = 1659177614;
static int16_t gBufferSize = 256;

/**
 * @tc.name      : strftime_0100
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 0
 */
void strftime_0100(void)
{
    for (int32_t i = 0; i < (int32_t)(sizeof(test_asctime_data) / sizeof(test_asctime_data[0])); i++) {
        const char *handlerChar = test_handle_path(test_asctime_data[i].tz);
        if (!handlerChar) {
            t_error("strftime_0100 failed: handlerChar is NULL\n");
            continue;
        }

        setenv("TZ", handlerChar, 1);
        tzset();
        char buffer[gBufferSize];
        struct tm *timeptr = localtime(&gTime);
        if (!timeptr) {
            EXPECT_PTRNE("strftime_0100", timeptr, NULL);
            return;
        }
        size_t count = strftime(buffer, sizeof(buffer) - 1, "%c", timeptr);
        EXPECT_TRUE("strftime_0100", count > 0);
        EXPECT_STREQ("strftime_0100", buffer, test_asctime_data[i].result);
    }
}

/**
 * @tc.name      : strftime_0200
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 0
 */
void strftime_0200(void)
{
    for (int32_t i = 0; i < (int32_t)(sizeof(test_strftime_data) / sizeof(test_strftime_data[0])); i++) {
        const char *handlerChar = test_handle_path(test_strftime_data[i].tz);
        if (!handlerChar) {
            t_error("strftime_0200 failed: handlerChar is NULL\n");
            continue;
        }

        setenv("TZ", handlerChar, 1);
        tzset();
        struct tm *timeptr = localtime(&gTime);
        if (!timeptr) {
            EXPECT_PTRNE("strftime_0200", timeptr, NULL);
            return;
        }
        char buffer[gBufferSize];
        size_t count = strftime(buffer, sizeof(buffer) - 1, "%c %Z%z", timeptr);
        EXPECT_TRUE("strftime_0200", count > 0);
        EXPECT_STREQ("strftime_0200", buffer, test_strftime_data[i].result);
    }
}

/**
 * @tc.name      : strftime_0300
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 0
 */
void strftime_0300(void)
{
    const char *handlerChar = test_handle_path("Pacific/Pitcairn");
    if (!handlerChar) {
        t_error("strftime_0300 failed: handlerChar is NULL\n");
        return;
    }

    setenv("TZ", handlerChar, 1);
    tzset();
    struct tm *timeptr = localtime(&gTime);
    if (!timeptr) {
        EXPECT_PTRNE("strftime_0300", timeptr, NULL);
        return;
    }
    char buffer[gBufferSize];
    size_t count = strftime(buffer, sizeof(buffer) - 1, "%k", timeptr);
    EXPECT_TRUE("strftime_0300", count > 0);
    EXPECT_STREQ("strftime_0300", buffer, " 2");
}

/**
 * @tc.name      : strftime_0400
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 0
 */
void strftime_0400(void)
{
    const char *handlerChar = test_handle_path("Asia/Shanghai");
    if (!handlerChar) {
        t_error("strftime_0400 failed: handlerChar is NULL\n");
        return;
    }

    setenv("TZ", handlerChar, 1);
    tzset();
    struct tm *timeptr = localtime(&gTime);
    if (!timeptr) {
        EXPECT_PTRNE("strftime_0400", timeptr, NULL);
        return;
    }
    char buffer[gBufferSize];
    size_t count = strftime(buffer, sizeof(buffer) - 1, "%k", timeptr);
    EXPECT_TRUE("strftime_0400", count > 0);
    EXPECT_STREQ("strftime_0400", buffer, "18");
}

/**
 * @tc.name      : strftime_0500
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 0
 */
void strftime_0500(void)
{
    const char *handlerChar = test_handle_path("Asia/Shanghai");
    if (!handlerChar) {
        t_error("strftime_0500 failed: handlerChar is NULL\n");
        return;
    }

    setenv("TZ", handlerChar, 1);
    tzset();
    struct tm *timeptr = localtime(&gTime);
    if (!timeptr) {
        EXPECT_PTRNE("strftime_0500", timeptr, NULL);
        return;
    }
    char buffer[gBufferSize];
    size_t count = strftime(buffer, sizeof(buffer) - 1, "%I", timeptr);
    EXPECT_TRUE("strftime_0500", count > 0);
    EXPECT_STREQ("strftime_0500", buffer, "06");
}

/**
 * @tc.name      : strftime_0600
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 0
 */
void strftime_0600(void)
{
    const char *handlerChar = test_handle_path("Asia/Shanghai");
    if (!handlerChar) {
        t_error("strftime_0600 failed: handlerChar is NULL\n");
        return;
    }

    setenv("TZ", handlerChar, 1);
    tzset();
    struct tm *timeptr = localtime(&gTime);
    if (!timeptr) {
        EXPECT_PTRNE("strftime_0600", timeptr, NULL);
        return;
    }
    char buffer[gBufferSize];
    size_t count = strftime(buffer, sizeof(buffer) - 1, "%P", timeptr);
    EXPECT_TRUE("strftime_0600", count > 0);
    EXPECT_STREQ("strftime_0600", buffer, "pm");
}

/**
 * @tc.name      : strftime_0700
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 0
 */
void strftime_0700(void)
{
    const char *handlerChar = test_handle_path("Asia/Shanghai");
    if (!handlerChar) {
        t_error("strftime_0700 failed: handlerChar is NULL\n");
        return;
    }

    setenv("TZ", handlerChar, 1);
    tzset();
    struct tm *timeptr = localtime(&gTime);
    if (!timeptr) {
        EXPECT_PTRNE("strftime_0700", timeptr, NULL);
        return;
    }
    char buffer[gBufferSize];
    size_t count = strftime(buffer, sizeof(buffer) - 1, "%v", timeptr);
    EXPECT_TRUE("strftime_0700", count > 0);
    EXPECT_STREQ("strftime_0700", buffer, "30-Jul-2022");
}

/**
 * @tc.name      : strftime_0800
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 0
 */
void strftime_0800(void)
{
    const char *handlerChar = test_handle_path("Asia/Shanghai");
    if (!handlerChar) {
        t_error("strftime_0800 failed: handlerChar is NULL\n");
        return;
    }

    setenv("TZ", handlerChar, 1);
    tzset();
    struct tm *timeptr = localtime(&gTime);
    if (!timeptr) {
        EXPECT_PTRNE("strftime_0800", timeptr, NULL);
        return;
    }
    char buffer[gBufferSize];
    size_t count = strftime(buffer, sizeof(buffer) - 1, "%j", timeptr);
    EXPECT_TRUE("strftime_0800", count > 0);
    EXPECT_STREQ("strftime_0800", buffer, "211");
}

/**
 * @tc.name      : strftime_0900
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 0
 */
void strftime_0900(void)
{
    const char *handlerChar = test_handle_path("Asia/Shanghai");
    if (!handlerChar) {
        t_error("strftime_0900 failed: handlerChar is NULL\n");
        return;
    }

    setenv("TZ", handlerChar, 1);
    tzset();
    struct tm *timeptr = localtime(&gTime);
    if (!timeptr) {
        EXPECT_PTRNE("strftime_0900", timeptr, NULL);
        return;
    }
    char buffer[gBufferSize];
    size_t count = strftime(buffer, sizeof(buffer) - 1, "%l", timeptr);
    EXPECT_TRUE("strftime_0900", count > 0);
    EXPECT_STREQ("strftime_0900", buffer, " 6");
}

/**
 * @tc.name      : strftime_1000
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 0
 */
void strftime_1000(void)
{
    const char *handlerChar = test_handle_path("Asia/Shanghai");
    if (!handlerChar) {
        t_error("strftime_1000 failed: handlerChar is NULL\n");
        return;
    }

    setenv("TZ", handlerChar, 1);
    tzset();
    struct tm *timeptr = localtime(&gTime);
    if (!timeptr) {
        EXPECT_PTRNE("strftime_1000", timeptr, NULL);
        return;
    }
    timeptr->tm_mday = 31;
    timeptr->tm_mon = 11;
    timeptr->tm_year = 124;
    timeptr->tm_wday = 2;
    timeptr->tm_yday = 365;

    char buffer[gBufferSize];
    size_t count = strftime(buffer, sizeof(buffer) - 1, "%V", timeptr);
    EXPECT_TRUE("strftime_1000", count > 0);
    EXPECT_STREQ("strftime_1000", buffer, "01");
}

/**
 * @tc.name      : strftime_1100
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 0
 */
void strftime_1100(void)
{
    struct tm tm = {0};
    char buffer[gBufferSize];
    size_t count = strftime(buffer, sizeof(buffer) - 1, "%l", &tm);
    EXPECT_TRUE("strftime_1100", count > 0);
    EXPECT_STREQ("strftime_1100", buffer, "12");
}

/**
 * @tc.name      : strftime_1200
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 0
 */
void strftime_1200(void)
{
    const char *handlerChar = test_handle_path("Asia/Shanghai");
    if (!handlerChar) {
        t_error("strftime_1200 failed: handlerChar is NULL\n");
        return;
    }

    setenv("TZ", handlerChar, 1);
    tzset();
    struct tm *timeptr = localtime(&gTime);
    if (!timeptr) {
        EXPECT_PTRNE("strftime_1200", timeptr, NULL);
        return;
    }
    timeptr->tm_mday = 28;
    timeptr->tm_mon = 12;
    timeptr->tm_year = 200;
    timeptr->tm_wday = 1;
    timeptr->tm_yday = 362;

    char buffer[gBufferSize];
    size_t count = strftime(buffer, sizeof(buffer) - 1, "%G", timeptr);
    EXPECT_TRUE("strftime_1200", count > 0);
    EXPECT_STREQ("strftime_1200", buffer, "2101");
}

/**
 * @tc.name      : strftime_1300
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 1
 */
void strftime_1300(void)
{
    const char *handlerChar = test_handle_path("Asia/Shanghai");
    if (!handlerChar) {
        t_error("strftime_1300 failed: handlerChar is NULL\n");
        return;
    }

    setenv("TZ", handlerChar, 1);
    tzset();
    struct tm *timeptr = localtime(&gTime);
    if (!timeptr) {
        EXPECT_PTRNE("strftime_1300", timeptr, NULL);
        return;
    }
    timeptr->tm_mday = 28;
    timeptr->tm_mon = 12;
    timeptr->tm_year = 100;
    timeptr->tm_wday = 1;
    timeptr->tm_yday = 362;

    char buffer[gBufferSize];
    size_t count = strftime(buffer, sizeof(buffer) - 1, "%G", timeptr);
    EXPECT_TRUE("strftime_1300", count > 0);
    EXPECT_STREQ("strftime_1300", buffer, "2000");
}

/**
 * @tc.name      : strftime_1400
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 1
 */
void strftime_1400(void)
{
    const char *handlerChar = test_handle_path("Asia/Shanghai");
    if (!handlerChar) {
        t_error("strftime_1400 failed: handlerChar is NULL\n");
        return;
    }

    setenv("TZ", handlerChar, 1);
    tzset();
    struct tm *timeptr = localtime(&gTime);
    if (!timeptr) {
        EXPECT_PTRNE("strftime_1400", timeptr, NULL);
        return;
    }
    timeptr->tm_mday = 28;
    timeptr->tm_mon = 12;
    timeptr->tm_year = 101;
    timeptr->tm_wday = 1;
    timeptr->tm_yday = 362;

    char buffer[gBufferSize];
    size_t count = strftime(buffer, sizeof(buffer) - 1, "%G", timeptr);
    EXPECT_TRUE("strftime_1400", count > 0);
    EXPECT_STREQ("strftime_1400", buffer, "2002");
}

/**
 * @tc.name      : strftime_1500
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 1
 */
void strftime_1500(void)
{
    const char *handlerChar = test_handle_path("Asia/Shanghai");
    if (!handlerChar) {
        t_error("strftime_1500 failed: handlerChar is NULL\n");
        return;
    }

    setenv("TZ", handlerChar, 1);
    tzset();
    struct tm *timeptr = localtime(&gTime);
    if (!timeptr) {
        EXPECT_PTRNE("strftime_1500", timeptr, NULL);
        return;
    }

    char buffer[gBufferSize];
    size_t count = strftime(buffer, sizeof(buffer) - 1, "%B", timeptr);
    EXPECT_TRUE("strftime_1500", count > 0);
    EXPECT_STREQ("strftime_1500", buffer, "July");
}

/**
 * @tc.name      : strftime_1600
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 1
 */
void strftime_1600(void)
{
    const char *handlerChar = test_handle_path("Asia/Shanghai");
    if (!handlerChar) {
        t_error("strftime_1600 failed: handlerChar is NULL\n");
        return;
    }

    setenv("TZ", handlerChar, 1);
    tzset();
    struct tm *timeptr = localtime(&gTime);
    if (!timeptr) {
        EXPECT_PTRNE("strftime_1600", timeptr, NULL);
        return;
    }
    timeptr->tm_mon = 12;

    char buffer[gBufferSize];
    size_t count = strftime(buffer, sizeof(buffer) - 1, "%B", timeptr);
    EXPECT_TRUE("strftime_1600", count > 0);
    EXPECT_STREQ("strftime_1600", buffer, "-");
}

/**
 * @tc.name      : strftime_1700
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 1
 */
void strftime_1700(void)
{
    struct tm tm = {0};
    tm.tm_hour = 13;
    char buffer[gBufferSize];
    size_t count = strftime(buffer, sizeof(buffer) - 1, "%l", &tm);
    EXPECT_TRUE("strftime_1700", count > 0);
    EXPECT_STREQ("strftime_1700", buffer, " 1");
}

/**
 * @tc.name      : strftime_1800
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 1
 */
void strftime_1800(void)
{
    struct tm tm = {0};
    char buffer[gBufferSize];
    size_t count = strftime(buffer, sizeof(buffer) - 1, "%u", &tm);
    EXPECT_TRUE("strftime_1800", count > 0);
    EXPECT_STREQ("strftime_1800", buffer, "7");
}

/**
 * @tc.name      : strftime_1900
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 1
 */
void strftime_1900(void)
{
    struct tm tm = {0};
    tm.tm_wday = 3;
    char buffer[gBufferSize];
    size_t count = strftime(buffer, sizeof(buffer) - 1, "%u", &tm);
    EXPECT_TRUE("strftime_1900", count > 0);
    EXPECT_STREQ("strftime_1900", buffer, "3");
}

/**
 * @tc.name      : strftime_2000
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 1
 */
void strftime_2000(void)
{
    struct tm tm = {0};
    tm.tm_year = -1999;
    char buffer[gBufferSize];
    size_t count = strftime(buffer, sizeof(buffer) - 1, "%-y", &tm);
    EXPECT_TRUE("strftime_2000", count > 0);
    EXPECT_STREQ("strftime_2000", buffer, "99");
}

/**
 * @tc.name      : strftime_2100
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 1
 */
void strftime_2100(void)
{
    struct tm tm = {0};
    tm.tm_isdst = -1;
    char buffer[gBufferSize];
    size_t count = strftime(buffer, sizeof(buffer) - 1, "%z", &tm);
    EXPECT_TRUE("strftime_2100", count == 0);
    EXPECT_STREQ("strftime_2100", buffer, "");
}

/**
 * @tc.name      : strftime_2200
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 1
 */
void strftime_2200(void)
{
    struct tm tm = {0};
    tm.tm_isdst = -1;
    char buffer[gBufferSize];
    size_t count = strftime(buffer, sizeof(buffer) - 1, "%Z", &tm);
    EXPECT_TRUE("strftime_2200", count == 0);
    EXPECT_STREQ("strftime_2200", buffer, "");
}

/**
 * @tc.name      : strftime_2300
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 1
 */
void strftime_2300(void)
{
    struct tm tm = {0};
    char buffer[gBufferSize];
    size_t count = strftime(buffer, sizeof(buffer) - 1, "%E%", &tm);
    EXPECT_TRUE("strftime_2300", count > 0);
    EXPECT_STREQ("strftime_2300", buffer, "%");
}

/**
 * @tc.name      : strftime_2200
 * @tc.desc      : according to different time zones, format date
 * @tc.level     : Level 1
 */
void strftime_2400(void)
{
    struct tm tm = {0};
    char buffer[gBufferSize];
    size_t count = strftime(buffer, sizeof(buffer) - 1, "%O%", &tm);
    EXPECT_TRUE("strftime_2400", count > 0);
    EXPECT_STREQ("strftime_2400", buffer, "%");
}

int main(void)
{
    strftime_0100();
    strftime_0200();
    strftime_0300();
    strftime_0400();
    strftime_0500();
    strftime_0600();
    strftime_0700();
    strftime_0800();
    strftime_0900();
    strftime_1000();
    strftime_1100();
    strftime_1200();
    strftime_1300();
    strftime_1400();
    strftime_1500();
    strftime_1600();
    strftime_1700();
    strftime_1800();
    strftime_1900();
    strftime_2000();
    strftime_2100();
    strftime_2200();
    strftime_2300();
    strftime_2400();
    return t_status;
}