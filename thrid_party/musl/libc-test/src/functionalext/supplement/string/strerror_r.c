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

#define BUFSIZE 1024
const int32_t invalidParam = 1;

/**
 * @tc.name      : strerror_r_0100
 * @tc.desc      : Verify strerror_r process success
 * @tc.level     : Level 0
 */
void strerror_r_0100(void)
{
    char src[BUFSIZE] = "strerror_r_0100";
    int32_t ret = strerror_r(EDOM, src, BUFSIZE);
    EXPECT_EQ("strerror_r_0100", ret, 0);
}

/**
 * @tc.name      : strerror_r_0200
 * @tc.desc      : Verify strerror_r process success when thrid param is 0
 * @tc.level     : Level 1
 */
void strerror_r_0200(void)
{
    char src[BUFSIZE] = "strcasecmp_0200";
    int32_t ret = strerror_r(EDOM, src, 0);
    EXPECT_EQ("strerror_r_0200", ret, ERANGE);
}

/**
 * @tc.name      : strerror_r_0300
 * @tc.desc      : Verify strerror_r process success when thrid param is 1
 * @tc.level     : Level 1
 */
void strerror_r_0300(void)
{
    char src[BUFSIZE] = "strerror_r_0300";
    int32_t ret = strerror_r(EDOM, src, invalidParam);
    EXPECT_EQ("strerror_r_0300", ret, ERANGE);
}

int main(void)
{
    strerror_r_0100();
    strerror_r_0200();
    strerror_r_0300();
    return t_status;
}
