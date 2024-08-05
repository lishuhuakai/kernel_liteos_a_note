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
#include <wchar.h>
#include "functionalext.h"

#define SIZE 128
#define INIT_ERROR_VALUE 0x01

/**
 * @tc.name      : mbsinit_0100
 * @tc.desc      : Verify mbsinit process success and return nonzero value when st points to an mbstate_t object
 *                 that describes the initial transition state.
 * @tc.leve      : Level 0
 */
void mbsinit_0100(void)
{
    mbstate_t mbst;
    memset(&mbst, 0, sizeof(mbst));
    int ret = mbsinit(&mbst);
    EXPECT_NE("mbsinit_0100", ret, 0);
}

/**
 * @tc.name      : mbsinit_0200
 * @tc.desc      : Verify mbsinit process success and return nonzero value when st is null.
 * @tc.leve      : Level 0
 */
void mbsinit_0200(void)
{
    int ret = mbsinit(NULL);
    EXPECT_NE("mbsinit_0200", ret, 0);
}

/**
 * @tc.name      : mbsinit_0300
 * @tc.desc      : Verify mbsinit process success and return zero value when when st points to an mbstate_t object
 *                 that not describes the initial transition state.
 * @tc.leve      : Level 1
 */
void mbsinit_0300(void)
{
    mbstate_t mbst;
    memset(&mbst, INIT_ERROR_VALUE, sizeof(mbst));
    int ret = mbsinit(&mbst);
    EXPECT_EQ("mbsinit_0300", ret, 0);
}

int main(void)
{
    mbsinit_0100();
    mbsinit_0200();
    mbsinit_0300();
    return t_status;
}