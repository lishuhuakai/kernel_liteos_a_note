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

#include <fcntl.h>
#include <sys/klog.h>
#include "functionalext.h"

#define SYSLOG_ACTION_SIZE_BUFFER (10)

/**
 * @tc.name      : klogctl_0100
 * @tc.desc      : Verify klogctl process success.
 * @tc.level     : Level 0
 */
void klogctl_0100(void)
{
    int ret = klogctl(SYSLOG_ACTION_SIZE_BUFFER, 0, 0);
    EXPECT_MT("klogctl_0200", ret, 0);
}

/**
 * @tc.name      : klogctl_0200
 * @tc.desc      : Verify klogctl process fail bacause param type is invalid.
 * @tc.level     : Level 0
 */
void klogctl_0200(void)
{
    int ret = klogctl(-1, 0, 0);
    EXPECT_EQ("klogctl_0200", ret, -1);
}

int main(void)
{
    klogctl_0100();
    klogctl_0200();
    return t_status;
}
