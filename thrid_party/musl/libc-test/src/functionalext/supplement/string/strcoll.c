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

/**
 * @tc.name      : strcoll_0100
 * @tc.desc      : Verify strcoll process success when fist string equal second stirng
 * @tc.level     : Level 0
 */
void strcoll_0100(void)
{
    int32_t ret = strcoll("aBcDeF", "aBcDeF");
    EXPECT_EQ("strcoll_0100", ret, 0);
}

/**
 * @tc.name      : strcoll_0200
 * @tc.desc      : Verify strcoll process success when fist string less than second stirng
 * @tc.level     : Level 1
 */
void strcoll_0200(void)
{
    int32_t ret = strcoll("", "AbCdEf");
    EXPECT_LT("strcoll_0200", ret, 0);
}

/**
 * @tc.name      : strcoll_0300
 * @tc.desc      : Verify strcoll process success when fist string more than second stirng
 * @tc.level     : Level 1
 */
void strcoll_0300(void)
{
    int32_t ret = strcoll("AbCdEf", "");
    EXPECT_MT("strcoll_0300", ret, 0);
}

/**
 * @tc.name      : strcoll_0400
 * @tc.desc      : Verify strcoll process success when fist string is null second stirng is null
 * @tc.level     : Level 1
 */
void strcoll_0400(void)
{
    int32_t ret = strcoll("", "");
    EXPECT_EQ("strcoll_0400", ret, 0);
}

int main(void)
{
    strcoll_0100();
    strcoll_0200();
    strcoll_0300();
    strcoll_0400();
    return t_status;
}
