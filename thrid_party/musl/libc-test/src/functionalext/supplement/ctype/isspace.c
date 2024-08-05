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

#include <ctype.h>
#include "functionalext.h"

/*
 * @tc.name      : isspace_0100
 * @tc.desc      : Verify that the input character is a space (the parameter is a space)
 * @tc.level     : Level 0
 */
void isspace_0100(void)
{
    int ret = isspace(' ');
    EXPECT_NE("isspace_0100", ret, 0);
}

/*
 * @tc.name      : isspace_0200
 * @tc.desc      : Verify that the input character is a space (parameter is \r)
 * @tc.level     : Level 1
 */
void isspace_0200(void)
{
    int ret = isspace('\r');
    EXPECT_NE("isspace_0200", ret, 0);
}

/*
 * @tc.name      : isspace_0300
 * @tc.desc      : Verify that the input character is a space (parameter is \n)
 * @tc.level     : Level 1
 */
void isspace_0300(void)
{
    int ret = isspace('\n');
    EXPECT_NE("isspace_0300", ret, 0);
}

/*
 * @tc.name      : isspace_0400
 * @tc.desc      : EVerify that the input character is a space (parameter is \v)
 * @tc.level     : Level 1
 */
void isspace_0400(void)
{
    int ret = isspace('\v');
    EXPECT_NE("isspace_0400", ret, 0);
}

/*
 * @tc.name      : isspace_0500
 * @tc.desc      : Verify that the input character is a space (parameter is \f)
 * @tc.level     : Level 1
 */
void isspace_0500(void)
{
    int ret = isspace('\f');
    EXPECT_NE("isspace_0500", ret, 0);
}

/*
 * @tc.name      : isspace_0600
 * @tc.desc      : Verify that the input character is not a space (parameter is 'a')
 * @tc.level     : Level 2
 */
void isspace_0600(void)
{
    int ret = isspace('a');
    EXPECT_EQ("isspace_0600", ret, 0);
}

/*
 * @tc.name      : isspace_0700
 * @tc.desc      : Verify that the input character is not a space (parameter is '6')
 * @tc.level     : Level 2
 */
void isspace_0700(void)
{
    int ret = isspace('6');
    EXPECT_EQ("isspace_0700", ret, 0);
}

/*
 * @tc.name      : isspace_0800
 * @tc.desc      : Verify that the input character is not a space (parameter is '#')
 * @tc.level     : Level 2
 */
void isspace_0800(void)
{
    int ret = isspace('#');
    EXPECT_EQ("isspace_0800", ret, 0);
}

/*
 * @tc.name      : isspace_0900
 * @tc.desc      : Verify the number of characters in the ascii code table
 * @tc.level     : Level 1
 */
void isspace_0900(void)
{
    int total = 0;
    for (int i = 0; i < 128; i++) {
        int ret = isspace((char)i);
        if (ret) {
            total++;
        }
    }
    EXPECT_EQ("isspace_0900", total, 6);
}

int main(void)
{
    isspace_0100();
    isspace_0200();
    isspace_0300();
    isspace_0400();
    isspace_0500();
    isspace_0600();
    isspace_0700();
    isspace_0800();
    isspace_0900();

    return t_status;
}