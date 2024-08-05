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

#include <errno.h>
#include <sys/stat.h>

#include "test.h"

/**
 * @tc.name      : umask_0100
 * @tc.desc      : set file mode creation mask
 * @tc.level     : Level 0
 */
void umask_0100(void)
{
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
    mode_t result = umask(mode);

    result = umask(result);
    if (result != mode) {
        t_error("%s failed: result = %o\n", __func__, result);
    }
}

/**
 * @tc.name      : umask_0200
 * @tc.desc      : set an invalid file mode creation mask
 * @tc.level     : Level 2
 */
void umask_0200(void)
{
    mode_t mode = -1;
    mode_t result = umask(mode);

    result = umask(result);
    if (result == mode) {
        t_error("%s failed: result = %o\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    umask_0100();
    umask_0200();

    return t_status;
}
