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
 * @tc.name      : setresgid_0100
 * @tc.desc      : set real, effective, and saved group ID
 * @tc.level     : Level 0
 */
void setresgid_0100(void)
{
    gid_t crgid = 0;
    gid_t cegid = 0;
    gid_t csgid = 0;
    int result = getresgid(&crgid, &cegid, &csgid);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    gid_t srgid = 1;
    gid_t segid = 2;
    gid_t ssgid = 3;
    result = setresgid(srgid, segid, ssgid);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    gid_t rgid = 0;
    gid_t egid = 0;
    gid_t sgid = 0;
    result = getresgid(&rgid, &egid, &sgid);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    if ((rgid != srgid) || (egid != segid) || (sgid != ssgid)) {
        t_error("%s failed: rgid = %d\n", __func__, rgid);
        t_error("%s failed: egid = %d\n", __func__, egid);
        t_error("%s failed: sgid = %d\n", __func__, sgid);
    }

    result = setresgid(crgid, cegid, csgid);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }
}

/**
 * @tc.name      : setresgid_0200
 * @tc.desc      : set real, effective, and saved group ID with the current values
 * @tc.level     : Level 1
 */
void setresgid_0200(void)
{
    gid_t crgid = 0;
    gid_t cegid = 0;
    gid_t csgid = 0;
    int result = getresgid(&crgid, &cegid, &csgid);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    gid_t srgid = crgid;
    gid_t segid = crgid;
    gid_t ssgid = crgid;
    result = setresgid(srgid, segid, ssgid);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    gid_t rgid = 0;
    gid_t egid = 0;
    gid_t sgid = 0;
    result = getresgid(&rgid, &egid, &sgid);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    if ((rgid != srgid) || (egid != segid) || (sgid != ssgid)) {
        t_error("%s failed: rgid = %d\n", __func__, rgid);
        t_error("%s failed: egid = %d\n", __func__, egid);
        t_error("%s failed: sgid = %d\n", __func__, sgid);
    }
}

int main(int argc, char *argv[])
{
    setresgid_0100();
    setresgid_0200();

    return t_status;
}
