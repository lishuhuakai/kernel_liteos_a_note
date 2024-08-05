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
#include "strptime_data.h"

static int16_t gBufferSize = 256;
static time_t gTime = 1659177614;
static int16_t gYearBase = 1900;

/**
 * @tc.name      : strptime_0100
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_0100(void)
{
    for ( int32_t i = 0; i < (int32_t)(sizeof(test_asctime_data) / sizeof(test_asctime_data[0])); i++) {
        const char *handlerChar = test_handle_path(test_asctime_data[i].tz);
        if (!handlerChar) {
            t_error("strptime_0100 failed: handlerChar is NULL\n");
            continue;
        }

        setenv("TZ", handlerChar, 1);
        tzset();
        char buffer[gBufferSize];
        struct tm *timeptr = localtime(&gTime);
        if (!timeptr) {
            EXPECT_PTRNE("strptime_0100", timeptr, NULL);
            return;
        }
        size_t cnt = strftime(buffer, sizeof(buffer) - 1, "%c", timeptr);
        EXPECT_TRUE("strptime_0100", cnt > 0);

        struct tm tmTime = {0};
        strptime(buffer, "%c", &tmTime);
        char *result = asctime(&tmTime);
        if (result == NULL) {
            EXPECT_FALSE("strptime_0100", result == NULL);
            return;
        }
        result[strlen(result) - 1] = 0x00;
        EXPECT_STREQ("strptime_0100", test_asctime_data[i].result, result);
    }
}

/**
 * @tc.name      : strptime_0200
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_0200(void)
{
    for ( int32_t i = 0; i < (int32_t)(sizeof(test_strptime_data)/sizeof(test_strptime_data[0])); i++) {
        const char *handlerChar = test_handle_path(test_strptime_data[i].tz);
        if (!handlerChar) {
            t_error("strptime_0200 failed: handlerChar is NULL\n");
            continue;
        }

        setenv("TZ", handlerChar, 1);
        tzset();
        char buffer[gBufferSize];
        struct tm *timeptr = localtime(&gTime);
        if (!timeptr) {
            EXPECT_TRUE("strptime_0200", timeptr == NULL);
            return;
        }
        size_t len = strftime(buffer, sizeof(buffer) - 1, "%c %Z%z", timeptr);
        EXPECT_TRUE("strptime_0200", len > 0);
        struct tm tmTime = {0};
        strptime(buffer, "%c %Z%z", &tmTime);
        char buffResult[gBufferSize];

        int cnt = sprintf(buffResult, "%d-%d-%d %d:%d:%d wday=%d,yday=%d,isdst=%d,gmtoff=%ld,zone=%s",
            (tmTime.tm_year+gYearBase), tmTime.tm_mon, tmTime.tm_mday, tmTime.tm_hour,
            tmTime.tm_min, tmTime.tm_sec, tmTime.tm_wday, tmTime.tm_yday, tmTime.tm_isdst,
            tmTime.tm_gmtoff, tmTime.tm_zone);
        EXPECT_TRUE("strptime_0200", cnt > 0);
        EXPECT_STREQ("strptime_0200", test_strptime_data[i].result, buffResult);
    }
}

/**
 * @tc.name      : strptime_0300
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_0300(void)
{
    char *buffer = "2022-04-10";
    struct tm tmTime = {0};
    strptime(buffer, "%F", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%04d-%02d-%02d",
        (tmTime.tm_year+gYearBase), tmTime.tm_mon + 1, tmTime.tm_mday);
    EXPECT_TRUE("strptime_0300", cnt > 0);
    EXPECT_STREQ("strptime_0300", buffer, buffResult);
}

/**
 * @tc.name      : strptime_0400
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_0400(void)
{
    char *buffer = "23";
    struct tm tmTime = {0};
    strptime(buffer, "%g", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%d", tmTime.tm_year);
    EXPECT_TRUE("strptime_0400", cnt > 0);
    EXPECT_STREQ("strptime_0400", buffer, buffResult);
}

/**
 * @tc.name      : strptime_0500
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_0500(void)
{
    const char *buffer = "16";
    struct tm tmTime = {0};
    strptime(buffer, " %k", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%d", tmTime.tm_hour);
    EXPECT_TRUE("strptime_0500", cnt > 0);
    EXPECT_STREQ("strptime_0500", buffer, buffResult);
}

/**
 * @tc.name      : strptime_0600
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_0600(void)
{
    const char *buffer = " 4";
    struct tm tmTime = {0};
    strptime(buffer, " %l", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%d", tmTime.tm_hour);
    EXPECT_TRUE("strptime_0600", cnt > 0);
    EXPECT_STREQ("strptime_0600", "4", buffResult);
}

/**
 * @tc.name      : strptime_0700
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_0700(void)
{
    const char *buffer = "1659177614";
    const char *handlerChar = test_handle_path(test_asctime_data[0].tz);
    if (!handlerChar) {
        t_error("strptime_0700 failed: handlerChar is NULL\n");
        return;
    }

    setenv("TZ", handlerChar, 1);
    time_t second = 0;
    struct tm tmTime = {0};
    strptime(buffer, "%s", &tmTime);
    second = mktime(&tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%lld", second);
    EXPECT_TRUE("strptime_0700", cnt > 0);
    EXPECT_STREQ("strptime_0700", buffer, buffResult);
}

/**
 * @tc.name      : strptime_0800
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_0800(void)
{
    const char *buffer = "1";
    struct tm tmTime = {0};
    strptime(buffer, "%u", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%d", tmTime.tm_wday);
    EXPECT_TRUE("strptime_0800", cnt > 0);
    EXPECT_STREQ("strptime_0800", buffer, buffResult);
}

/**
 * @tc.name      : strptime_0900
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_0900(void)
{
    const char *buffer = "30-Oct-2021";
    struct tm tmTime = {0};
    strptime(buffer, "%v", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%d-%d-%d",
        (tmTime.tm_year+gYearBase), tmTime.tm_mon, tmTime.tm_mday);
    EXPECT_TRUE("strptime_0900", cnt > 0);
    EXPECT_STREQ("strptime_0900", "2021-9-30", buffResult);
}

/**
 * @tc.name      : strptime_1000
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 1
 */
void strptime_1000(void)
{
    const char *buffer = "2021-01-23";
    struct tm tmTime = {0};
    char *result = strptime(buffer, "%G", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%s", result);
    EXPECT_TRUE("strptime_1000", cnt > 0);
    EXPECT_STREQ("strptime_1000", "-01-23", buffResult);
}

/**
 * @tc.name      : strptime_1100
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 1
 */
void strptime_1100(void)
{
    const char *buffer = "23";
    struct tm tmTime = {0};
    strptime(buffer, "%j", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%d", tmTime.tm_yday);
    EXPECT_TRUE("strptime_1100", cnt > 0);
    EXPECT_STREQ("strptime_1100", "22", buffResult);
}

/**
 * @tc.name      : strptime_1200
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_1200(void)
{
    const char *buffer = "am";
    const char *handlerChar = test_handle_path("Asia/Shanghai");
    if (!handlerChar) {
        t_error("strptime_1200 failed: handlerChar is NULL\n");
        return;
    }

    setenv("TZ", handlerChar, 1);
    tzset();
    struct tm *timeptr = localtime(&gTime);
    if (!timeptr) {
        EXPECT_PTRNE("strptime_1200", timeptr, NULL);
        return;
    }
    strptime(buffer, "%P", timeptr);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%d", timeptr->tm_hour);
    EXPECT_TRUE("strptime_1200", cnt > 0);
    EXPECT_STREQ("strptime_1200", "6", buffResult);
}

/**
 * @tc.name      : strptime_1300
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_1300(void)
{
    const char *buffer = "pm";
    const char *handlerChar = test_handle_path("Asia/Shanghai");
    if (!handlerChar) {
        t_error("strptime_1300 failed: handlerChar is NULL\n");
        return;
    }

    setenv("TZ", handlerChar, 1);
    tzset();
    struct tm *timeptr = localtime(&gTime);
    if (!timeptr) {
        EXPECT_PTRNE("strptime_1300", timeptr, NULL);
        return;
    }
    strptime(buffer, "%P", timeptr);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%d", timeptr->tm_hour);
    EXPECT_TRUE("strptime_1300", cnt > 0);
    EXPECT_STREQ("strptime_1300", "18", buffResult);
}

/**
 * @tc.name      : strptime_1400
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_1400(void)
{
    const char *buffer = "30-Oct-2021";
    struct tm tmTime = {0};
    char *result = strptime(buffer, "%U", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%s", result);
    EXPECT_TRUE("strptime_1400", cnt > 0);
    EXPECT_STREQ("strptime_1400", "-Oct-2021", buffResult);
}

/**
 * @tc.name      : strptime_1500
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_1500(void)
{
    const char *buffer = "1";
    struct tm tmTime = {0};
    char *result = strptime(buffer, "%w", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%d", tmTime.tm_wday);
    EXPECT_TRUE("strptime_1500", cnt > 0);
    EXPECT_STREQ("strptime_1500", "1", buffResult);
}

/**
 * @tc.name      : strptime_1600
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_1600(void)
{
    const char *buffer = "Oct-30-2021";
    struct tm tmTime = {0};
    strptime(buffer, "%v", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%d-%d-%d",
        (tmTime.tm_year+gYearBase), tmTime.tm_mon, tmTime.tm_mday);
    EXPECT_TRUE("strptime_1600", cnt > 0);
    EXPECT_STREQ("strptime_1600", "1900-0-0", buffResult);
}

/**
 * @tc.name      : strptime_1700
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 0
 */
void strptime_1700(void)
{
    const char *buffer = "16-Spring";
    struct tm tmTime = {0};
    char *result = strptime(buffer, "%V", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%s", result);
    EXPECT_TRUE("strptime_1700", cnt > 0);
    EXPECT_STREQ("strptime_1700", "-Spring", buffResult);
}

/**
 * @tc.name      : strptime_1800
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 1
 */
void strptime_1800(void)
{
    const char *buffer = "+03";
    struct tm tmTime = {0};
    char *result = strptime(buffer, "%Z", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%s", tmTime.__tm_zone);
    EXPECT_TRUE("strptime_1800", cnt > 0);
    EXPECT_STREQ("strptime_1800", "+03", buffResult);
}

/**
 * @tc.name      : strptime_1900
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 1
 */
void strptime_1900(void)
{
    const char *buffer = "-03";
    struct tm tmTime = {0};
    char *result = strptime(buffer, "%Z", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%s", tmTime.__tm_zone);
    EXPECT_TRUE("strptime_1900", cnt > 0);
    EXPECT_STREQ("strptime_1900", "-03", buffResult);
}

/**
 * @tc.name      : strptime_2000
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 2
 */
void strptime_2000(void)
{
    const char *buffer = "Oct-30-2021";
    struct tm tmTime = {0};
    char *result = strptime(buffer, "test", &tmTime);
    EXPECT_FALSE("strptime_2000", result);
}

/**
 * @tc.name      : strptime_2100
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 1
 */
void strptime_2100(void)
{
    const char *buffer = "2022-4-10";
    struct tm tmTime = {0};
    char *result = strptime(buffer, "%+2F", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%d", tmTime.tm_mday);
    EXPECT_TRUE("strptime_2100", cnt > 0);
    EXPECT_STREQ("strptime_2100", "10", buffResult);
}

/**
 * @tc.name      : strptime_2200
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 2
 */
void strptime_2200(void)
{
    const char *buffer = "";
    struct tm tmTime = {0};
    char *result = strptime(buffer, "%c", &tmTime);
    EXPECT_FALSE("strptime_2200", result);
}

/**
 * @tc.name      : strptime_2300
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 1
 */
void strptime_2300(void)
{
    const char *buffer = "2022";
    struct tm tmTime = {0};
    char *result = strptime(buffer, "%C", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%s", result);
    EXPECT_TRUE("strptime_2300", cnt > 0);
    EXPECT_STREQ("strptime_2300", "22", buffResult);
}

/**
 * @tc.name      : strptime_2400
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 2
 */
void strptime_2400(void)
{
    const char *buffer = "";
    struct tm tmTime = {0};
    char *result = strptime(buffer, "%D", &tmTime);
    EXPECT_FALSE("strptime_2400", result);
}

/**
 * @tc.name      : strptime_2500
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 2
 */
void strptime_2500(void)
{
    const char *buffer = "";
    struct tm tmTime = {0};
    char *result = strptime(buffer, "%F", &tmTime);
    EXPECT_FALSE("strptime_2500", result);
}

/**
 * @tc.name      : strptime_2600
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 1
 */
void strptime_2600(void)
{
    const char *buffer = " 1";
    struct tm tmTime = {0};
    char *result = strptime(buffer, "%n%t", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%s", result);
    EXPECT_TRUE("strptime_2600", cnt > 0);
    EXPECT_STREQ("strptime_2600", "1", buffResult);
}

/**
 * @tc.name      : strptime_2700
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 1
 */
void strptime_2700(void)
{
    const char *buffer = "08:38:20";
    struct tm tmTime = {0};
    char *result = strptime(buffer, "%r", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%d:%d:%d", tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec);
    EXPECT_TRUE("strptime_2700", cnt > 0);
    EXPECT_STREQ("strptime_2700", "8:38:20", buffResult);
}

/**
 * @tc.name      : strptime_2800
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 2
 */
void strptime_2800(void)
{
    const char *buffer = "";
    struct tm tmTime = {0};
    char *result = strptime(buffer, "%r", &tmTime);
    EXPECT_FALSE("strptime_2800", result);
}

/**
 * @tc.name      : strptime_2900
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 2
 */
void strptime_2900(void)
{
    const char *buffer = "";
    struct tm tmTime = {0};
    char *result = strptime(buffer, "%R", &tmTime);
    EXPECT_FALSE("strptime_2900", result);
}

/**
 * @tc.name      : strptime_3000
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 2
 */
void strptime_3000(void)
{
    const char *buffer = "+1";
    struct tm tmTime = {0};
    char *result = strptime(buffer, "%s", &tmTime);
    EXPECT_FALSE("strptime_3000", result);
}

/**
 * @tc.name      : strptime_3100
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 2
 */
void strptime_3100(void)
{
    const char *buffer = "";
    struct tm tmTime = {0};
    char *result = strptime(buffer, "%T", &tmTime);
    EXPECT_FALSE("strptime_3100", result);
}

/**
 * @tc.name      : strptime_3200
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 2
 */
void strptime_3200(void)
{
    const char *buffer = "";
    struct tm tmTime = {0};
    char *result = strptime(buffer, "%u", &tmTime);
    EXPECT_FALSE("strptime_3200", result);
}

/**
 * @tc.name      : strptime_3300
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 2
 */
void strptime_3300(void)
{
    const char *buffer = "";
    struct tm tmTime = {0};
    char *result = strptime(buffer, "%V", &tmTime);
    EXPECT_FALSE("strptime_3300", result);
}

/**
 * @tc.name      : strptime_3400
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 1
 */
void strptime_3400(void)
{
    const char *buffer = "04/10/22";
    struct tm tmTime = {0};
    char *result = strptime(buffer, "%x", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%d/%d/%d", (tmTime.tm_mon + 1), tmTime.tm_mday, (tmTime.tm_year + gYearBase));
    EXPECT_TRUE("strptime_3400", cnt > 0);
    EXPECT_STREQ("strptime_3400", "4/10/2022", buffResult);
}

/**
 * @tc.name      : strptime_3500
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 2
 */
void strptime_3500(void)
{
    const char *buffer = "";
    struct tm tmTime = {0};
    char *result = strptime(buffer, "%x", &tmTime);
    EXPECT_FALSE("strptime_3500", result);
}

/**
 * @tc.name      : strptime_3600
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 1
 */
void strptime_3600(void)
{
    const char *buffer = "08:10:20";
    struct tm tmTime = {0};
    char *result = strptime(buffer, "%X", &tmTime);
    char buffResult[gBufferSize];
    int cnt = sprintf(buffResult, "%d:%d:%d", tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec);
    EXPECT_TRUE("strptime_3600", cnt > 0);
    EXPECT_STREQ("strptime_3600", "8:10:20", buffResult);
}

/**
 * @tc.name      : strptime_3700
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 2
 */
void strptime_3700(void)
{
    const char *buffer = "";
    struct tm tmTime = {0};
    char *result = strptime(buffer, "%X", &tmTime);
    EXPECT_FALSE("strptime_3700", result);
}

/**
 * @tc.name      : strptime_3800
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 2
 */
void strptime_3800(void)
{
    const char *buffer = "";
    struct tm tmTime = {0};
    char *result = strptime(buffer, "%%", &tmTime);
    EXPECT_FALSE("strptime_3800", result);
}

/**
 * @tc.name      : strptime_3900
 * @tc.desc      : according to different time zones, convert a string to a time
 * type according to a specific time format
 * @tc.level     : Level 2
 */
void strptime_3900(void)
{
    const char *buffer = "";
    struct tm tmTime = {0};
    char *result = strptime(buffer, "%&", &tmTime);
    EXPECT_FALSE("strptime_3900", result);
}

int main(void)
{
    strptime_0100();
    strptime_0200();
    strptime_0300();
    strptime_0400();
    strptime_0500();
    strptime_0600();
    strptime_0700();
    strptime_0800();
    strptime_0900();
    strptime_1000();
    strptime_1100();
    strptime_1200();
    strptime_1300();
    strptime_1400();
    strptime_1500();
    strptime_1600();
    strptime_1700();
    strptime_1800();
    strptime_1900();
    strptime_2000();
    strptime_2100();
    strptime_2200();
    strptime_2300();
    strptime_2400();
    strptime_2500();
    strptime_2600();
    strptime_2700();
    strptime_2800();
    strptime_2900();
    strptime_3000();
    strptime_3100();
    strptime_3200();
    strptime_3300();
    strptime_3400();
    strptime_3500();
    strptime_3600();
    strptime_3700();
    strptime_3800();
    strptime_3900();
    return t_status;
}