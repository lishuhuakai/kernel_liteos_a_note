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

#include <stdio.h>
#include <unistd.h>
#include "test.h"

/**
 * @tc.name      : setresuid_0100
 * @tc.desc      : Test calling setresuid to set the real, effective and saved user or group ID
 * @tc.level     : Level 0
 */
void setresuid_0100(void)
{
    uid_t uid = 0;
    int result = setresuid(uid, uid, uid);
    if (result != 0) {
        t_error("%s setresuid error get result is %d are not 0\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    setresuid_0100();
    return t_status;
}