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

#include <netdb.h>
#include "functionalext.h"

/*
 * @tc.name      : gai_strerror_0100
 * @tc.desc      : Verify that the corresponding error message can be returned (parameter is EAI_BADFLAGS)
 * @tc.level     : Level 0
 */
void gai_strerror_0100(void)
{
    const char *result = gai_strerror(EAI_BADFLAGS);
    EXPECT_TRUE(gai_strerror_0100, strcmp(result, "Invalid flags\0") == 0);
}

/*
 * @tc.name      : gai_strerror_0200
 * @tc.desc      : Verify that the corresponding error message can be returned (parameter is EAI_NONAME)
 * @tc.level     : Level 0
 */
void gai_strerror_0200(void)
{
    const char *result = gai_strerror(EAI_NONAME);
    EXPECT_TRUE(gai_strerror_0200, strcmp(result, "Name does not resolve\0") == 0);
}

/*
 * @tc.name      : gai_strerror_0300
 * @tc.desc      : Verify that the corresponding error message can be returned (parameter is EAI_AGAIN)
 * @tc.level     : Level 0
 */
void gai_strerror_0300(void)
{
    const char *result = gai_strerror(EAI_AGAIN);
    EXPECT_TRUE(gai_strerror_0300, strcmp(result, "Try again\0") == 0);
}

/*
 * @tc.name      : gai_strerror_0400
 * @tc.desc      : Verify that the corresponding error message can be returned (parameter is EAI_FAIL)
 * @tc.level     : Level 0
 */
void gai_strerror_0400(void)
{
    const char *result = gai_strerror(EAI_FAIL);
    EXPECT_TRUE(gai_strerror_0400, strcmp(result, "Non-recoverable error\0") == 0);
}

/*
 * @tc.name      : gai_strerror_0500
 * @tc.desc      : Verify that the corresponding error message can be returned (parameter is -5)
 * @tc.level     : Level 1
 */
void gai_strerror_0500(void)
{
    const char *result = gai_strerror(-5);
    EXPECT_TRUE(gai_strerror_0500, strcmp(result, "Unknown error\0") == 0);
}

/*
 * @tc.name      : gai_strerror_0600
 * @tc.desc      : Verify that the corresponding error message can be returned (parameter is EAI_FAMILY)
 * @tc.level     : Level 0
 */
void gai_strerror_0600(void)
{
    const char *result = gai_strerror(EAI_FAMILY);
    EXPECT_TRUE(gai_strerror_0600, strcmp(result, "Unrecognized address family or invalid length\0") == 0);
}

/*
 * @tc.name      : gai_strerror_0700
 * @tc.desc      : Verify that the corresponding error message can be returned (parameter is EAI_SOCKTYPE)
 * @tc.level     : Level 0
 */
void gai_strerror_0700(void)
{
    const char *result = gai_strerror(EAI_SOCKTYPE);
    EXPECT_TRUE(gai_strerror_0700, strcmp(result, "Unrecognized socket type\0") == 0);
}

/*
 * @tc.name      : gai_strerror_0800
 * @tc.desc      : Verify that the corresponding error message can be returned (parameter is EAI_SERVICE)
 * @tc.level     : Level 0
 */
void gai_strerror_0800(void)
{
    const char *result = gai_strerror(EAI_SERVICE);
    EXPECT_TRUE(gai_strerror_0800, strcmp(result, "Unrecognized service\0") == 0);
}

/*
 * @tc.name      : gai_strerror_0900
 * @tc.desc      : Verify that the corresponding error message can be returned (parameter is -9)
 * @tc.level     : Level 1
 */
void gai_strerror_0900(void)
{
    const char *result = gai_strerror(-9);
    EXPECT_TRUE(gai_strerror_0900, strcmp(result, "Unknown error\0") == 0);
}

/*
 * @tc.name      : gai_strerror_1000
 * @tc.desc      : Verify that the corresponding error message can be returned (parameter is EAI_MEMORY)
 * @tc.level     : Level 0
 */
void gai_strerror_1000(void)
{
    const char *result = gai_strerror(EAI_MEMORY);
    EXPECT_TRUE(gai_strerror_1000, strcmp(result, "Out of memory\0") == 0);
}

/*
 * @tc.name      : gai_strerror_1100
 * @tc.desc      : Verify that the corresponding error message can be returned (parameter is EAI_SYSTEM)
 * @tc.level     : Level 0
 */
void gai_strerror_1100(void)
{
    const char *result = gai_strerror(EAI_SYSTEM);
    EXPECT_TRUE(gai_strerror_1100, strcmp(result, "System error\0") == 0);
}

/*
 * @tc.name      : gai_strerror_1200
 * @tc.desc      : Verify that the corresponding error message can be returned (parameter is EAI_OVERFLOW)
 * @tc.level     : Level 0
 */
void gai_strerror_1200(void)
{
    const char *result = gai_strerror(EAI_OVERFLOW);
    EXPECT_TRUE(gai_strerror_1200, strcmp(result, "Overflow\0") == 0);
}

/*
 * @tc.name      : gai_strerror_1300
 * @tc.desc      : Verify that the corresponding error message can be returned (parameter is -13)
 * @tc.level     : Level 1
 */
void gai_strerror_1300(void)
{
    const char *result = gai_strerror(-13);
    EXPECT_TRUE(gai_strerror_1300, strcmp(result, "Unknown error\0") == 0);
}

int main(int argc, char *argv[])
{
    gai_strerror_0100();
    gai_strerror_0200();
    gai_strerror_0300();
    gai_strerror_0400();
    gai_strerror_0500();
    gai_strerror_0600();
    gai_strerror_0700();
    gai_strerror_0800();
    gai_strerror_0900();
    gai_strerror_1000();
    gai_strerror_1100();
    gai_strerror_1200();
    gai_strerror_1300();

    return t_status;
}