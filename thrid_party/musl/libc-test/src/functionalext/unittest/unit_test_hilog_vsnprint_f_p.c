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

#include "test.h"
#include <vsnprintf_s_p.h>
#include "functionalext.h"

#define MAX_LOG_LEN 1024 /* maximum length of a log, include '\0' */
#define MAX_TAG_LEN 32   /* log tag size, include '\0' */
#define SECUREC_STRING_MAX_LEN 0x7fffffffUL

typedef void (*TEST_FUN)(void);

static int vsprintf_test(char *strDest, size_t destMax, size_t count, int priv, const char *fmt, ...)
{
    int ret;
    va_list ap;
    va_start(ap, fmt);
    ret = vsnprintfp_s(strDest, destMax, count, priv, fmt, ap);
    va_end(ap);
    return ret;
}

/**
 * @tc.name     : vsnprintfp_s_0010
 * @tc.desc     : test vsnprintf normal condition
 * @tc.level    : Level 0
 */
static void vsnprintfp_s_0010(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "MUSL";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, true, fmt);
    EXPECT_TRUE("vsnprintfp_s_0010", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0010", buf, "MUSL");
}

/**
 * @tc.name     : vsnprintfp_s_0020
 * @tc.desc     : test vsnprintf both param buf and fmt are NULL
 * @tc.level    : Level 2
 */
static void vsnprintfp_s_0020(void)
{
    char *buf = NULL;
    char *fmt = NULL;
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, true, fmt);
    EXPECT_TRUE("vsnprintfp_s_0020", ret < 0);
}

/**
 * @tc.name     : vsnprintfp_s_0030
 * @tc.desc     : test vsnprintf param destMax is smaller than count
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0030(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "MUSL";
    int ret = vsprintf_test(buf, MAX_LOG_LEN - 1, MAX_LOG_LEN, true, fmt);
    EXPECT_TRUE("vsnprintfp_s_0030", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0030", buf, "MUSL");
}

/**
 * @tc.name     : vsnprintfp_s_0040
 * @tc.desc     : test vsnprintf param fmt with %s
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0040(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%s";
    int ret = vsprintf_test(buf, MAX_LOG_LEN - 1, MAX_LOG_LEN, false, fmt, "MUSL");
    EXPECT_TRUE("vsnprintfp_s_0040", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0040", buf, "MUSL");
}

/**
 * @tc.name     : vsnprintfp_s_0050
 * @tc.desc     : test vsnprintf param destMax is 0
 * @tc.level    : Level 2
 */
static void vsnprintfp_s_0050(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "MUSL";
    int ret = vsprintf_test(buf, 0, MAX_LOG_LEN - 1, true, fmt);
    EXPECT_TRUE("vsnprintfp_s_0050", ret < 0);
    EXPECT_STREQ("vsnprintfp_s_0050", buf, "");
}

/**
 * @tc.name     : vsnprintfp_s_0060
 * @tc.desc     : test vsnprintf param destMax is greater than SECUREC_STRING_MAX_LEN
 * @tc.level    : Level 2
 */
static void vsnprintfp_s_0060(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "MUSL";
    int ret = vsprintf_test(buf, SECUREC_STRING_MAX_LEN + 1, MAX_LOG_LEN - 1, true, fmt);
    EXPECT_TRUE("vsnprintfp_s_0060", ret < 0);
    EXPECT_STREQ("vsnprintfp_s_0060", buf, "");
}

/**
 * @tc.name     : vsnprintfp_s_0070
 * @tc.desc     : test vsnprintf param count is greater than SECUREC_STRING_MAX_LEN
 * @tc.level    : Level 2
 */
static void vsnprintfp_s_0070(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "MUSL";
    int ret = vsprintf_test(buf, MAX_LOG_LEN - 1, SECUREC_STRING_MAX_LEN + 1, true, fmt);
    EXPECT_TRUE("vsnprintfp_s_0070", ret < 0);
    EXPECT_STREQ("vsnprintfp_s_0070", buf, "");
}

/**
 * @tc.name     : vsnprintfp_s_0080
 * @tc.desc     : test vsnprintf param fmt is NULL
 * @tc.level    : Level 2
 */
static void vsnprintfp_s_0080(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = NULL;
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, true, fmt);
    EXPECT_TRUE("vsnprintfp_s_0080", ret < 0);
}

/**
 * @tc.name     : vsnprintfp_s_0090
 * @tc.desc     : test vsnprintf param count is less than buf's size
 * @tc.level    : Level 2
 */
static void vsnprintfp_s_0090(void)
{
    char buf[2] = {0};
    char *fmt = "%s";
    int ret = vsprintf_test(buf, 1, 1, false, fmt, "test_test");
    EXPECT_TRUE("vsnprintfp_s_0090", ret < 0);
}

/**
 * @tc.name     : vsnprintfp_s_0100
 * @tc.desc     : test vsnprintf param fmt is ""
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0100(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, true, fmt);
    EXPECT_TRUE("vsnprintfp_s_0100", ret == 0);
    EXPECT_STREQ("vsnprintfp_s_0100", buf, "");
}

/**
 * @tc.name     : vsnprintfp_s_0110
 * @tc.desc     : test vsnprintf param fmt is "%02d"
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0110(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%02d";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 2);
    EXPECT_TRUE("vsnprintfp_s_0110", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0110", buf, "02");
}

/**
 * @tc.name     : vsnprintfp_s_0120
 * @tc.desc     : test vsnprintf param fmt is "%p"
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0120(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%p";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, &buf[0]);
    EXPECT_TRUE("vsnprintfp_s_0120", ret > 0);
    EXPECT_TRUE("vsnprintfp_s_0120", strlen(buf) > 0);
}

/**
 * @tc.name     : vsnprintfp_s_0130
 * @tc.desc     : test vsnprintf param fmt is "%S"
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0130(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%S";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, "Test");
    EXPECT_TRUE("vsnprintfp_s_0130", ret < 0);
}

/**
 * @tc.name     : vsnprintfp_s_0140
 * @tc.desc     : test vsnprintf param fmt is "%c"
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0140(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%c";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 'a');
    EXPECT_TRUE("vsnprintfp_s_0140", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0140", buf, "a");
}

/**
 * @tc.name     : vsnprintfp_s_0150
 * @tc.desc     : test vsnprintf param fmt is "%lu"
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0150(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%lu";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 21);
    EXPECT_TRUE("vsnprintfp_s_0150", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0150", buf, "21");
}

/**
 * @tc.name     : vsnprintfp_s_0160
 * @tc.desc     : test vsnprintf param fmt is "%.3f, %2.2f"
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0160(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%.3f, %2.2f";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 3.14, 2.2);
    EXPECT_TRUE("vsnprintfp_s_0160", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0160", buf, "3.140, 2.20");
}

/**
 * @tc.name     : vsnprintfp_s_0170
 * @tc.desc     : test vsnprintf param fmt is "%% %n\n"
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0170(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%% %n\n";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, true, fmt);
    EXPECT_TRUE("vsnprintfp_s_0170", ret < 0);
}

/**
 * @tc.name     : vsnprintfp_s_0180
 * @tc.desc     : test vsnprintf param fmt is "%F"
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0180(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%F";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 3.14);
    EXPECT_TRUE("vsnprintfp_s_0180", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0180", buf, "3.140000");
}

/**
 * @tc.name     : vsnprintfp_s_0190
 * @tc.desc     : test vsnprintf param fmt is "%x"
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0190(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%x";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 61);
    EXPECT_TRUE("vsnprintfp_s_0190", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0190", buf, "3d");
}

/**
 * @tc.name     : vsnprintfp_s_0200
 * @tc.desc     : test vsnprintf param fmt is "%X"
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0200(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%X";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 61);
    EXPECT_TRUE("vsnprintfp_s_0200", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0200", buf, "3D");
}

/**
 * @tc.name     : vsnprintfp_s_0210
 * @tc.desc     : test vsnprintf param fmt is "%5d*"
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0210(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "*%5d*";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 1024);
    EXPECT_TRUE("vsnprintfp_s_0210", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0210", buf, "* 1024*");
}

/**
 * @tc.name     : vsnprintfp_s_0220
 * @tc.desc     : test vsnprintf param fmt is "%% %n\n"
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0220(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%#0*X";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 8, 128);
    EXPECT_TRUE("vsnprintfp_s_0220", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0220", buf, "0X000080");
}

/**
 * @tc.name     : vsnprintfp_s_0230
 * @tc.desc     : test vsnprintf param fmt is "%+d"
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0230(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%+d";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 22);
    EXPECT_TRUE("vsnprintfp_s_0230", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0230", buf, "+22");
}

/**
 * @tc.name     : vsnprintfp_s_0240
 * @tc.desc     : test vsnprintf param fmt is "%-d"
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0240(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%-d";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 22);
    EXPECT_TRUE("vsnprintfp_s_0240", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0240", buf, "22");
}

/**
 * @tc.name     : vsnprintfp_s_0250
 * @tc.desc     : test vsnprintf param fmt is "%ho"
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0250(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%ho";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 9);
    EXPECT_TRUE("vsnprintfp_s_0250", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0250", buf, "11");
}

/**
 * @tc.name     : vsnprintfp_s_0260
 * @tc.desc     : test vsnprintf param fmt is "%Le"
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0260(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%Le";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 10000);
    EXPECT_TRUE("vsnprintfp_s_0260", ret == 0);
}

/**
 * @tc.name     : vsnprintfp_s_0270
 * @tc.desc     : test vsnprintf param fmt is "%{public}d"
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0270(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%{public}d";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, true, fmt, 22);
    EXPECT_TRUE("vsnprintfp_s_0270", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0270", buf, "22");
}

/**
 * @tc.name     : vsnprintfp_s_0280
 * @tc.desc     : test vsnprintf param fmt is "%{private}d"
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0280(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%{private}d";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, true, fmt, 22);
    EXPECT_TRUE("vsnprintfp_s_0280", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0280", buf, "<private>");
}

/**
 * @tc.name     : vsnprintfp_s_0290
 * @tc.desc     : test vsnprintf param fmt is "%C"
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0290(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%C";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 'a');
    EXPECT_TRUE("vsnprintfp_s_0290", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0290", buf, "a");
}

/**
 * @tc.name     : vsnprintfp_s_0300
 * @tc.desc     : test vsnprintf param fmt is "%c"
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0300(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%c";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 'a');
    EXPECT_TRUE("vsnprintfp_s_0300", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0300", buf, "a");
}

/**
 * @tc.name     : vsnprintfp_s_0310
 * @tc.desc     : test vsnprintf param fmt is "%I64 %hho"
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0310(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%I64o %hho";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 2, 3);
    EXPECT_TRUE("vsnprintfp_s_0310", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0310", buf, "6040000000003 66");
}

/**
 * @tc.name     : vsnprintfp_s_0320
 * @tc.desc     : test vsnprintf param fmt is "%.3f"
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0320(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%.3f";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 3.14);
    EXPECT_TRUE("vsnprintfp_s_0320", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0320", buf, "3.140");
}

/**
 * @tc.name     : vsnprintfp_s_0330
 * @tc.desc     : test vsnprintf param fmt is "%{private}f"
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0330(void)
{
    char buf[2] = {0};
    char *fmt = "%{private}f";
    int ret = vsprintf_test(buf, 2, 1, false, fmt, 3.14);
    EXPECT_TRUE("vsnprintfp_s_0330", ret < 0);
    EXPECT_STREQ("vsnprintfp_s_0330", buf, "3");
}

/**
 * @tc.name     : vsnprintfp_s_0340
 * @tc.desc     : test vsnprintf param fmt is "% d"
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0340(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "% d";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 2);
    EXPECT_TRUE("vsnprintfp_s_0340", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0340", buf, " 2");
}

/**
 * @tc.name     : vsnprintfp_s_0350
 * @tc.desc     : test vsnprintf param fmt is "*%t*"
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0350(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "*%t*";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt);
    EXPECT_TRUE("vsnprintfp_s_0350", ret < 0);
}

/**
 * @tc.name     : vsnprintfp_s_0360
 * @tc.desc     : test vsnprintf param fmt is "%ld"
 * @tc.level    : Level 0
 */
static void vsnprintfp_s_0360(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%ld";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 333);
    EXPECT_TRUE("vsnprintfp_s_0360", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0360", buf, "333");
}

/**
 * @tc.name     : vsnprintfp_s_0370
 * @tc.desc     : test vsnprintf param fmt is "%V" 
 * @tc.level    : Level 2
 */
static void vsnprintfp_s_0370(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%V";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 9);
    EXPECT_TRUE("vsnprintfp_s_0370", ret < 0);
}

/**
 * @tc.name     : vsnprintfp_s_0380
 * @tc.desc     : test vsnprintf param fmt is "%{public}wc" 
 * @tc.level    : Level 0
 */
static void vsnprintfp_s_0380(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%{public}wc";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 'a');
    EXPECT_TRUE("vsnprintfp_s_0380", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0380", buf, "a");
}

/**
 * @tc.name     : vsnprintfp_s_0390
 * @tc.desc     : test vsnprintf param fmt is "%I" 
 * @tc.level    : Level 0
 */
static void vsnprintfp_s_0390(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%I";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 22);
    EXPECT_TRUE("vsnprintfp_s_0390", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0390", buf, "I");
}

/**
 * @tc.name     : vsnprintfp_s_0400
 * @tc.desc     : test vsnprintf param fmt is "%I64d %I32d" 
 * @tc.level    : Level 0
 */
static void vsnprintfp_s_0400(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%I64d %I32d";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 22, 33);
    EXPECT_TRUE("vsnprintfp_s_0400", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0400", buf, "94489280545 942945337");
}

/**
 * @tc.name     : vsnprintfp_s_0410
 * @tc.desc     : test vsnprintf param fmt is "%Id %Ii %Io %Iu %Ix %IX" 
 * @tc.level    : Level 0
 */
static void vsnprintfp_s_0410(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%Id %Ii %Io %Iu %Ix %IX";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 22, 33, 44, 55, 66, 77);
    EXPECT_TRUE("vsnprintfp_s_0410", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0410", buf, "22 33 54 55 42 4D");
}

/**
 * @tc.name     : vsnprintfp_s_0420
 * @tc.desc     : test vsnprintf param fmt is "%0c" 
 * @tc.level    : Level 0
 */
static void vsnprintfp_s_0420(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%0c";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 'c');
    EXPECT_TRUE("vsnprintfp_s_0420", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0420", buf, "c");
}

/**
 * @tc.name     : vsnprintfp_s_0430
 * @tc.desc     : test vsnprintf param fmt is "%hhd" 
 * @tc.level    : Level 0
 */
static void vsnprintfp_s_0430(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%hhd";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 1024);
    EXPECT_TRUE("vsnprintfp_s_0430", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0430", buf, "0");
}

/**
 * @tc.name     : vsnprintfp_s_0440
 * @tc.desc     : test vsnprintf param fmt is "%#o" 
 * @tc.level    : Level 0
 */
static void vsnprintfp_s_0440(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%#o";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 64);
    EXPECT_TRUE("vsnprintfp_s_0440", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0440", buf, "0100");
}

/**
 * @tc.name     : vsnprintfp_s_0450
 * @tc.desc     : test vsnprintf param fmt is "%#o"
 * @tc.level    : Level 0
 */
static void vsnprintfp_s_0450(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%{public}ws";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, "test");
    EXPECT_TRUE("vsnprintfp_s_0450", ret < 0);
}

/**
 * @tc.name     : vsnprintfp_s_0460
 * @tc.desc     : test vsnprintf param fmt is "%123123d"
 * @tc.level    : Level 2
 */
static void vsnprintfp_s_0460(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%123123d";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 1024);
    EXPECT_TRUE("vsnprintfp_s_0460", ret < 0);
}

/**
 * @tc.name     : vsnprintfp_s_0470
 * @tc.desc     : test vsnprintf param fmt is "%+123123d"
 * @tc.level    : Level 2
 */
static void vsnprintfp_s_0470(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%+123123d";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 1024);
    EXPECT_TRUE("vsnprintfp_s_0470", ret < 0);
}

/**
 * @tc.name     : vsnprintfp_s_0480
 * @tc.desc     : param count < PRIVATE_STR_LEN(9) and format is {private}
 * @tc.level    : Level 2
 */
static void vsnprintfp_s_0480(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%{private}d";
    int ret = vsprintf_test(buf, 8, 7, true, fmt, 22);
    EXPECT_TRUE("vsnprintfp_s_0480", ret < 0);
}

/**
 * @tc.name     : vsnprintfp_s_0490
 * @tc.desc     : test vsnprintf param fmt is "%d" and arg is (char)128
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0490(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%hhd";
    char num = 128;
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, num);
    EXPECT_TRUE("vsnprintfp_s_0490", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0490", buf, "-128");
}

/**
 * @tc.name     : vsnprintfp_s_0500
 * @tc.desc     : test vsnprintf param fmt is "%d" and arg is (char)128
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0500(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%*d";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 1, 1024);
    EXPECT_TRUE("vsnprintfp_s_0500", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0500", buf, "1024");
}

/**
 * @tc.name     : vsnprintfp_s_0510
 * @tc.desc     : test vsnprintf param fmt is "%I63d"
 * @tc.level    : Level 2
 */
static void vsnprintfp_s_0510(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%I63d";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 22);
    EXPECT_TRUE("vsnprintfp_s_0510", ret > 0);
}

/**
 * @tc.name     : vsnprintfp_s_0520
 * @tc.desc     : test vsnprintf param fmt is "%I31d"
 * @tc.level    : Level 2
 */
static void vsnprintfp_s_0520(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%I31d";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 22);
    EXPECT_TRUE("vsnprintfp_s_0520", ret > 0);
}

/**
 * @tc.name     : vsnprintfp_s_0530
 * @tc.desc     : test vsnprintf param fmt is "%.g"
 * @tc.level    : Level 1
 */
static void vsnprintfp_s_0530(void)
{
    char buf[MAX_LOG_LEN] = {0};
    char *fmt = "%.g";
    int ret = vsprintf_test(buf, MAX_LOG_LEN, MAX_LOG_LEN - 1, false, fmt, 0.12000);
    EXPECT_TRUE("vsnprintfp_s_0530", ret > 0);
    EXPECT_STREQ("vsnprintfp_s_0530", buf, "0.1");
}

TEST_FUN G_Fun_Array[] = {
    vsnprintfp_s_0010, 
    vsnprintfp_s_0020, 
    vsnprintfp_s_0030, 
    vsnprintfp_s_0040, 
    vsnprintfp_s_0050,
    vsnprintfp_s_0060, 
    vsnprintfp_s_0070, 
    vsnprintfp_s_0080, 
    vsnprintfp_s_0090, 
    vsnprintfp_s_0100,
    vsnprintfp_s_0110, 
    vsnprintfp_s_0120, 
    vsnprintfp_s_0130, 
    vsnprintfp_s_0140, 
    vsnprintfp_s_0150,
    vsnprintfp_s_0160, 
    vsnprintfp_s_0170, 
    vsnprintfp_s_0180, 
    vsnprintfp_s_0190, 
    vsnprintfp_s_0200,
    vsnprintfp_s_0210, 
    vsnprintfp_s_0220, 
    vsnprintfp_s_0230, 
    vsnprintfp_s_0240, 
    vsnprintfp_s_0250,
    vsnprintfp_s_0260, 
    vsnprintfp_s_0270, 
    vsnprintfp_s_0280, 
    vsnprintfp_s_0290, 
    vsnprintfp_s_0300,
    vsnprintfp_s_0310, 
    vsnprintfp_s_0320, 
    vsnprintfp_s_0330, 
    vsnprintfp_s_0340, 
    vsnprintfp_s_0350,
    vsnprintfp_s_0360, 
    vsnprintfp_s_0370, 
    vsnprintfp_s_0380, 
    vsnprintfp_s_0390, 
    vsnprintfp_s_0400,
    vsnprintfp_s_0410, 
    vsnprintfp_s_0420, 
    vsnprintfp_s_0430, 
    vsnprintfp_s_0440, 
    vsnprintfp_s_0450,
    vsnprintfp_s_0460, 
    vsnprintfp_s_0470, 
    vsnprintfp_s_0480, 
    vsnprintfp_s_0490, 
    vsnprintfp_s_0500,
    vsnprintfp_s_0510, 
    vsnprintfp_s_0520, 
    vsnprintfp_s_0530,
};

int main(void)
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }
    return t_status;
}
