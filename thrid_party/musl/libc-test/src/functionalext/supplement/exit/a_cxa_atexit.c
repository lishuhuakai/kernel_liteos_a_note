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
#include "functionalext.h"

extern int __cxa_atexit(void (*func)(void *), void *arg, void *dso);
const int32_t INVALID_LEN = -1;
const int32_t INIT_LEN = 0;
const int32_t INCREASE_LEN = 1;
const int32_t COUNT = 32;
const int32_t SLOT_LEN = 10;
void exitFunc(void *arg){};

/**
 * @tc.name:      cxa_atexit_0100
 * @tc.desc:      Verify cxa_atexit process success when second and third args is null.
 * @tc.level:     level 1.
 */
void cxa_atexit_0100(void)
{
    int32_t ret = __cxa_atexit(exitFunc, NULL, NULL);
    EXPECT_EQ("cxa_atexit_0100", ret, INIT_LEN);
}

/**
 * @tc.name:      cxa_atexit_0200
 * @tc.desc:      Verify cxa atexit process success when third args is null and slot less than COUNT.
 * @tc.level:     level 0.
 */
void cxa_atexit_0200(void)
{
    int32_t slot = SLOT_LEN;
    int32_t var = INIT_LEN;
    int32_t ret = __cxa_atexit(exitFunc, &var, NULL);
    EXPECT_LT("cxa_atexit_0200", slot + INCREASE_LEN, COUNT);
    EXPECT_EQ("cxa_atexit_0200", ret, 0);
}

/**
 * @tc.name:      cxa_atexit_0300
 * @tc.desc:      Verify cxa atexit process success when third args is null and slot equal COUNT.
 * @tc.level:     level 1.
 */
void cxa_atexit_0300(void)
{
    int32_t slot = COUNT;
    int32_t var = 0;
    int32_t ret = __cxa_atexit(exitFunc, &var, NULL);
    EXPECT_EQ("cxa_atexit_0300", slot, COUNT);
    EXPECT_EQ("cxa_atexit_0300", ret, 0);
}

int main(int argc, char *argv[])
{
    cxa_atexit_0100();
    cxa_atexit_0200();
    cxa_atexit_0300();
    return t_status;
}
