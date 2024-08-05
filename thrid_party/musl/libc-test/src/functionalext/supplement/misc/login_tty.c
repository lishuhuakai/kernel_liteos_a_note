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
#include <utmp.h>
#include "functionalext.h"

const int32_t fd = 3;

/**
 * @tc.name      : login_tty_0100.
 * @tc.desc      : Verify login_tty process success when param is -1 return -1.
 * @tc.level     : level 0
 */
void login_tty_0100(void)
{
    int ret = login_tty(-1);
    EXPECT_EQ("login_tty_0100", ret, -1);
}

int main(void)
{
    login_tty_0100();
    return t_status;
}
