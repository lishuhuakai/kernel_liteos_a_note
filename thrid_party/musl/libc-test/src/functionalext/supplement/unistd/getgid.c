/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include <sys/auxv.h>
#include <unistd.h>
#include "functionalext.h"

/*
 * @tc.name      : getgid_0100
 * @tc.desc      : Verify that the current user group ID can be obtained
 * @tc.level     : Level 0
 */
void getgid_0100(void)
{
    unsigned long int ret = getauxval(AT_GID);
    gid_t gid = getgid();
    EXPECT_EQ("getgid_0100", gid, ret);
}

int main(int argc, char *argv[])
{
    getgid_0100();
    return t_status;
}