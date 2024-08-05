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

#include <inttypes.h>
#include "functionalext.h"

/**
 * @tc.name      : imaxabs_0100
 * @tc.desc      : Verify imaxabs process success when param is positive integer
 * @tc.level     : Level 0
 */
void imaxabs_0100(void)
{
    intmax_t posmax_t = 1024;
    intmax_t ret = imaxabs(posmax_t);
    EXPECT_EQ("imaxabs_0100", ret, posmax_t);
}

/**
 * @tc.name      : imaxabs_0200
 * @tc.desc      : Verify imaxabs process success when param is negative integer
 * @tc.level     : Level 0
 */
void imaxabs_0200(void)
{
    intmax_t posmax_t = 1024;
    intmax_t negmax_t = -1024;
    intmax_t ret = imaxabs(negmax_t);
    EXPECT_EQ("imaxabs_0200", ret, posmax_t);
}

int main(void)
{
    imaxabs_0100();
    imaxabs_0200();
    return t_status;
}
