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

void f1()
{}
void f2()
{}

/**
 * @tc.name      : at_quick_exit_0100
 * @tc.desc      : Verify that the function pointed to can be registered (the number of registrations
 *                 is less than 32).
 * @tc.level     : Level 0
 */
void at_quick_exit_0100(void)
{
    int result = at_quick_exit(f1);
    quick_exit(0);
    EXPECT_EQ("at_quick_exit_0100", result, 0);
}

/**
 * @tc.name      : at_quick_exit_0200
 * @tc.desc      : Verify that the pointed function cannot be registered (the number of registrations
 *                 is greater than 32).
 * @tc.level     : Level 2
 */
void at_quick_exit_0200(void)
{
    int result;
    for (int i = 0; i < 32; i++) {
        at_quick_exit(f2);
    }
    result = at_quick_exit(f2);
    quick_exit(0);
    EXPECT_EQ("at_quick_exit_0200", result, -1);
}

int main(int argc, char *argv[])
{
    at_quick_exit_0100();
    at_quick_exit_0200();
    return t_status;
}
