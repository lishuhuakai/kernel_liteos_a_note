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

#include <stdlib.h>
#include <string.h>
#include "functionalext.h"

const int32_t compareLength = 3;

/**
 * @tc.name      : strncasecmp_0100
 * @tc.desc      : Verify strncasecmp process success when fist string equal second stirng
 * @tc.level     : Level 0
 */
void strncasecmp_0100(void)
{
    int32_t ret = strncasecmp("aBcDeF", "AbCdEf", compareLength);
    EXPECT_EQ("strncasecmp_0100", ret, 0);
}

/**
 * @tc.name      : strncasecmp_0200
 * @tc.desc      : Verify strncasecmp process success when fist string equal second stirng and compare length is 0
 * @tc.level     : Level 1
 */
void strncasecmp_0200(void)
{
    int32_t ret = strncasecmp("aBcDeF", "AbCdEf", 0);
    EXPECT_EQ("strncasecmp_0200", ret, 0);
}

/**
 * @tc.name      : strncasecmp_0300
 * @tc.desc      : Verify strcasecmp process success when fist string less than second stirng
 * @tc.level     : Level 1
 */
void strncasecmp_0300(void)
{
    int32_t ret = strncasecmp("", "AbCdEf", compareLength);
    EXPECT_LT("strcasecmp_0300", ret, 0);
}

/**
 * @tc.name      : strncasecmp_0400
 * @tc.desc      : Verify strncasecmp process success when fist string more than second stirng
 * @tc.level     : Level 1
 */
void strncasecmp_0400(void)
{
    int32_t ret = strncasecmp("AbCdEf", "", compareLength);
    EXPECT_MT("strncasecmp_0400", ret, 0);
}

/**
 * @tc.name      : strncasecmp_0500
 * @tc.desc      : Verify strncasecmp process success when fist string is null second stirng is null
 * @tc.level     : Level 1
 */
void strncasecmp_0500(void)
{
    int32_t ret = strncasecmp("", "", 3);
    EXPECT_EQ("strncasecmp_0500", ret, 0);
}

int main(void)
{
    strncasecmp_0100();
    strncasecmp_0200();
    strncasecmp_0300();
    strncasecmp_0400();
    strncasecmp_0500();
    return t_status;
}
