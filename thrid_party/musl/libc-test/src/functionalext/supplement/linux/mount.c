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

#include <errno.h>
#include <sys/mount.h>
#include "functionalext.h"

/**
 * @tc.name      : mount_0100
 * @tc.desc      : Mount the directory /mymount to the /tmp directory
 * @tc.level     : Level 2
 */
void mount_0100(void)
{
    const char special[] = "/mymount";
    const char target[] = "/tmp";
    int rev = mount(special, target, "", 0, "");
    EXPECT_EQ("mount_0100", rev, -1);
    if (rev == 0) {
        rev = umount(special);
        EXPECT_EQ("mount_0100", rev, 0);
    }
}

int main(void)
{
    mount_0100();
    return t_status;
}