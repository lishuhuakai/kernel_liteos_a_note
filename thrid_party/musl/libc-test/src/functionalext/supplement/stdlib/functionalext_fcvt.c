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

#include <stdlib.h>
#include <stdio.h>
#include "functionalext.h"

#define POSITIVE_NUM 0
#define NEGATIVE_NUM 1

/**
 * @tc.name      : fcvt_0100
 * @tc.desc      : Basic conversion test for fcvt.
 * @tc.level     : level 0
 */
void fcvt_0100()
{
    int decpt, sign;
    char *result = fcvt(123.456, 2, &decpt, &sign);
    printf("result==%s\n", result);
    EXPECT_STREQ("fcvt_0100", result, "12346");
    EXPECT_EQ("fcvt_0100_decpt", decpt, 3);
    EXPECT_EQ("fcvt_0100_sign", sign, POSITIVE_NUM);
}

void fcvt_0200()
{
    int decpt, sign;
    char *result = fcvt(123.454, 2, &decpt, &sign);
    printf("result==%s\n", result);
    EXPECT_STREQ("fcvt_0100", result, "12345");
    EXPECT_EQ("fcvt_0100_decpt", decpt, 3);
    EXPECT_EQ("fcvt_0100_sign", sign, POSITIVE_NUM);
}
/**
 * @tc.name      : fcvt_0200
 * @tc.desc      : Zero value conversion test for fcvt.
 * @tc.level     : level 0
 */
void fcvt_0300()
{
    int decpt, sign;
    char *result = fcvt(0.0, 2, &decpt, &sign);
    printf("result==%s\n", result);
    EXPECT_STREQ("fcvt_0300", result, "000");
    EXPECT_EQ("fcvt_0300_decpt", decpt, 1);
    EXPECT_EQ("fcvt_0300_sign", sign, POSITIVE_NUM);
}

/**
 * @tc.name      : fcvt_0300
 * @tc.desc      : Negative value conversion test for fcvt.
 * @tc.level     : level 0
 */
void fcvt_0400()
{
    int decpt, sign;
    char *result = fcvt(-123.456, 2, &decpt, &sign);
    printf("result==%s\n", result);
    EXPECT_STREQ("fcvt_0400", result, "12346");
    EXPECT_EQ("fcvt_0400_decpt", decpt, 3);
    EXPECT_EQ("fcvt_0400_sign", sign, NEGATIVE_NUM);
}

/**
 * @tc.name      : fcvt_0400
 * @tc.desc      : Test with a large number of digits after the decimal point.
 * @tc.level     : level 0
 */
void fcvt_0500()
{
    int decpt, sign;
    char *result = fcvt(123.456, 10, &decpt, &sign);
    printf("result==%s\n", result);
    // Here, the expected string is just an example, you might need to adjust based on actual behavior.
    EXPECT_STREQ("fcvt_0500", result, "1234560000000");
    EXPECT_EQ("fcvt_0500_decpt", decpt, 3);
    EXPECT_EQ("fcvt_0500_sign", sign, POSITIVE_NUM);
}

int main(void)
{
    fcvt_0100();
    fcvt_0200();
    fcvt_0300();
    fcvt_0400();
    fcvt_0500();
    return t_status;
}
