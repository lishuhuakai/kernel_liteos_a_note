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

#include <unistd.h>

#include "test.h"

/**
 * @tc.name      : setreuid_0100
 * @tc.desc      : set real and/or effective user ID
 * @tc.level     : Level 0
 */
void setreuid_0100(void)
{
    uid_t cuid = getuid();
    uid_t ceuid = geteuid();

    uid_t sruid = 1;
    uid_t seuid = 2;
    int result = setreuid(sruid, seuid);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    uid_t uid = getuid();
    uid_t euid = geteuid();
    if ((uid != sruid) || (euid != seuid)) {
        t_error("%s failed: uid = %d\n", __func__, uid);
        t_error("%s failed: euid = %d\n", __func__, euid);
    }

    result = setreuid(cuid, ceuid);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }
}

/**
 * @tc.name      : setreuid_0200
 * @tc.desc      : set real and/or effective user ID with the current effective user ID
 * @tc.level     : Level 1
 */
void setreuid_0200(void)
{
    uid_t cuid = getuid();
    uid_t ceuid = geteuid();

    uid_t sruid = cuid;
    uid_t seuid = ceuid;
    int result = setreuid(sruid, seuid);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    uid_t uid = getuid();
    uid_t euid = geteuid();
    if ((uid != sruid) || (euid != seuid)) {
        t_error("%s failed: uid = %d\n", __func__, uid);
        t_error("%s failed: euid = %d\n", __func__, euid);
    }
}

int main(int argc, char *argv[])
{
    setreuid_0100();
    setreuid_0200();

    return t_status;
}
