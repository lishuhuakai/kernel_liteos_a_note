/*
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

#include <stdio.h>
#include <sys/capability.h>
#include "test.h"

/**
 * @tc.name      : capset_0100
 * @tc.desc      : Set capabilities of thread
 * @tc.level     : Level 0
 */
void capset_0100(void)
{
    struct __user_cap_header_struct cap_header;
    struct __user_cap_data_struct cap_data;

    cap_header.pid = getpid();
    cap_header.version = _LINUX_CAPABILITY_VERSION_1;

    if (capget(&cap_header, &cap_data) < 0) {
        t_error("%s capget failed\n", __func__);
    }

    printf("capheader: %x  %d\n", cap_header.version, cap_header.pid);
    printf("capdata: %x  %x  %x\n", cap_data.effective, cap_data.permitted, cap_data.inheritable);

    __u32 cap_mask = 0;
    cap_mask |= (1 << CAP_NET_BIND_SERVICE);
    cap_data.effective = cap_mask;
    cap_data.permitted = cap_mask;
    cap_data.inheritable = 0;

    if (capset(&cap_header, &cap_data) < 0) {
        t_error("%s capset failed\n", __func__);
    }
    printf("%d\n", capget(&cap_header, &cap_data));
    printf("capheader: %x  %d\n", cap_header.version, cap_header.pid);
    printf("capdata: %x  %x  %x\n", cap_data.effective, cap_data.permitted, cap_data.inheritable);
}

int main(int argc, char *argv[])
{
    capset_0100();
    return t_status;
}