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

#include <syslog.h>

#include "test.h"

const int maskpri = LOG_UPTO(LOG_WARNING);

/**
 * @tc.name      : setlogmask_0100
 * @tc.desc      : set log priority mask
 * @tc.level     : Level 0
 */
void setlogmask_0100(void)
{
    int result = setlogmask(maskpri);

    result = setlogmask(result);

    if (result != maskpri) {
        t_error("%s failed: result = %d\n", __func__, result);
    }
}

/**
 * @tc.name      : setlogmask_0200
 * @tc.desc      : set log an invalid priority mask
 * @tc.level     : Level 2
 */
void setlogmask_0200(void)
{
    int result = setlogmask(0);

    if (result != setlogmask(result)) {
        t_error("%s failed: result = %d\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    setlogmask_0100();
    setlogmask_0200();

    return t_status;
}
