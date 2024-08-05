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
 * @tc.name      : setregid_0100
 * @tc.desc      : set real and/or effective group ID
 * @tc.level     : Level 0
 */
void setregid_0100(void)
{
    gid_t cgid = getgid();
    gid_t cegid = getegid();

    gid_t srgid = 1;
    gid_t segid = 2;
    int result = setregid(srgid, segid);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    gid_t gid = getgid();
    gid_t egid = getegid();
    if (gid != srgid || egid != segid) {
        t_error("%s failed: gid = %d\n", __func__, gid);
        t_error("%s failed: egid = %d\n", __func__, egid);
    }

    result = setregid(cgid, cegid);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }
}

/**
 * @tc.name      : setregid_0200
 * @tc.desc      : set real and/or effective group ID with the current effective group ID
 * @tc.level     : Level 1
 */
void setregid_0200(void)
{
    gid_t cgid = getgid();
    gid_t cegid = getegid();

    gid_t srgid = cgid;
    gid_t segid = cegid;
    int result = setregid(srgid, segid);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    gid_t gid = getgid();
    gid_t egid = getegid();
    if (gid != srgid || egid != segid) {
        t_error("%s failed: gid = %d\n", __func__, gid);
        t_error("%s failed: egid = %d\n", __func__, egid);
    }
}

int main(int argc, char *argv[])
{
    setregid_0100();
    setregid_0200();

    return t_status;
}
