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
#include <unistd.h>
#include "functionalext.h"

const int32_t GID1000 = 1000;

/**
 * @tc.name      : getresgid_0100
 * @tc.desc      : Verify getresgid process success
 * @tc.level     : Level 0
 */
void getresgid_0100(void)
{
    setgid(GID1000);
    gid_t gid_R, gid_E, gid_S;
    int ret = getresgid(&gid_R, &gid_E, &gid_S);
    EXPECT_EQ("getresgid_0100", ret, 0);
    EXPECT_EQ("getresgid_0100", gid_R, GID1000);
    EXPECT_EQ("getresgid_0100", gid_E, GID1000);
    EXPECT_EQ("getresgid_0100", gid_S, GID1000);
}

int main(void)
{
    getresgid_0100();
    return t_status;
}
