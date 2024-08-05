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
 * @tc.name      : setegid_0100
 * @tc.desc      : set effective group ID
 * @tc.level     : Level 0
 */
void setegid_0100(void)
{
    gid_t cegid = getegid();

    gid_t segid = 1;
    int result = setegid(segid);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    gid_t egid = getegid();
    if (egid != segid) {
        t_error("%s failed: egid = %d\n", __func__, egid);
    }

    result = setegid(cegid);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }
}

/**
 * @tc.name      : setegid_0200
 * @tc.desc      : set effective group ID with the current effective group ID
 * @tc.level     : Level 1
 */
void setegid_0200(void)
{
    gid_t cegid = getegid();

    gid_t segid = cegid;
    int result = setegid(segid);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    gid_t egid = getegid();
    if (egid != segid) {
        t_error("%s failed: egid = %d\n", __func__, egid);
    }
}

int main(int argc, char *argv[])
{
    setegid_0100();
    setegid_0200();

    return t_status;
}
