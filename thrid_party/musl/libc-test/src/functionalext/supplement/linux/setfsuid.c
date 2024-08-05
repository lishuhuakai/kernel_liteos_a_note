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

#include <sys/fsuid.h>

#include "test.h"

const gid_t fsuid = 1;
const gid_t invalid_fsuid = -1;

/**
 * @tc.name      : setfsuid_0100
 * @tc.desc      : set user identity used for filesystem checks
 * @tc.level     : Level 0
 */
void setfsuid_0100(void)
{
    int result = setfsuid(fsuid);
    if (result < 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    result = setfsuid(result);
    if (result != fsuid) {
        t_error("%s failed: result = %d\n", __func__, result);
    }
}

/**
 * @tc.name      : setfsuid_0200
 * @tc.desc      : set aninvalid user identity used for filesystem checks
 * @tc.level     : Level 2
 */
void setfsuid_0200(void)
{
    int result = setfsuid(invalid_fsuid);
    if (result < 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    result = setfsuid(result);
    if (result == fsuid) {
        t_error("%s failed: result = %d\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    // setfsuid_0100();
    // setfsuid_0200();

    return t_status;
}
