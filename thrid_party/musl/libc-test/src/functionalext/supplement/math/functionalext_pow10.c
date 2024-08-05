/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

#include <math.h>
#include <stdlib.h>
#include "functionalext.h"

/**
 * @tc.name      : pow10_0100
 * @tc.desc      : Validate that the result is 1-1e+15 when the input parameter is 0-15.
 * @tc.level     : Level 0
 */
void pow10_0100(void)
{
    double ret0 = pow10(0);
    EXPECT_EQ("pow10_0100", ret0, 1.0);
    double ret1 = pow10(1);
    EXPECT_EQ("pow10_0100", ret1, 10.0);
    double ret2 = pow10(2);
    EXPECT_EQ("pow10_0100", ret2, 100.0);
    double ret3 = pow10(3);
    EXPECT_EQ("pow10_0100", ret3, 1e+03);
    double ret4 = pow10(4);
    EXPECT_EQ("pow10_0100", ret4, 1e+04);
    double ret5 = pow10(5);
    EXPECT_EQ("pow10_0100", ret5, 1e+05);
    double ret6 = pow10(6);
    EXPECT_EQ("pow10_0100", ret6, 1e+06);
    double ret7 = pow10(7);
    EXPECT_EQ("pow10_0100", ret7, 1e+07);
    double ret8 = pow10(8);
    EXPECT_EQ("pow10_0100", ret8, 1e+08);
    double ret9 = pow10(9);
    EXPECT_EQ("pow10_0100", ret9, 1e+09);
    double ret10 = pow10(10);
    EXPECT_EQ("pow10_0100", ret10, 1e+10);
    double ret11 = pow10(11);
    EXPECT_EQ("pow10_0100", ret11, 1e+11);
    double ret12 = pow10(12);
    EXPECT_EQ("pow10_0100", ret12, 1e+12);
    double ret13 = pow10(13);
    EXPECT_EQ("pow10_0100", ret13, 1e+13);
    double ret14 = pow10(14);
    EXPECT_EQ("pow10_0100", ret14, 1e+14);
    double ret15 = pow10(15);
    EXPECT_EQ("pow10_0100", ret15, 1e+15);
}

/**
 * @tc.name      : pow10_0200
 * @tc.desc      : Validate that the result is 1e+16 when the input parameter is 16.
 * @tc.level     : Level 0
 */
void pow10_0200(void)
{
    double ret = pow10(16);
    EXPECT_EQ("pow10_0200", ret, 1e+16);
}

/**
 * @tc.name      : pow10_0300
 * @tc.desc      : Validate that the result is 0.1 when the input parameter is -1.
 * @tc.level     : Level 0
 */
void pow10_0300(void)
{
    double ret = pow10(-1);
    EXPECT_EQ("pow10_0300", ret, 0.1);
}

/**
 * @tc.name      : pow10_0400
 * @tc.desc      : Validate that the result is 1e+100 when the input parameter is 100.
 * @tc.level     : Level 0
 */
void pow10_0400(void)
{
    double ret = pow10(100);
    EXPECT_EQ("pow10_0400", ret, 1e+100);
}

/**
 * @tc.name      : pow10_0500
 * @tc.desc      : Validate that the result is 1e-100 when the input parameter is -100.
 * @tc.level     : Level 0
 */
void pow10_0500(void)
{
    double ret = pow10(-100);
    EXPECT_EQ("pow10_0500", ret, 1e-100);
}

int main(int argc, char *argv[])
{
    pow10_0100();
    pow10_0200();
    pow10_0300();
    pow10_0400();
    pow10_0500();
    return t_status;
}