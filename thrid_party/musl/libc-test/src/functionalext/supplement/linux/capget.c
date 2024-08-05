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
 * @tc.name      : capget_0100
 * @tc.desc      : Verify that this function can obtain process permissions
 * @tc.level     : Level 0
 */
void capget_0100(void)
{
    struct __user_cap_header_struct cap_header_data;
    cap_user_header_t cap_header = &cap_header_data;

    struct __user_cap_data_struct cap_data_data;
    cap_user_data_t cap_data = &cap_data_data;

    cap_header->pid = getpid();
    cap_header->version = _LINUX_CAPABILITY_VERSION_1;

    int result = capget(cap_header, cap_data);
    if (result < 0) {
        t_error("%s capget failed\n", __func__);
    }

    printf("Cap data 0x%x, 0x%x, 0x%x \n", cap_data->effective, cap_data->permitted, cap_data->inheritable);
}

/**
 * @tc.name      : capget_0200
 * @tc.desc      : Verify that this function could not obtain process permissions when parameter is invalid
 * @tc.level     : Level 2
 */
void capget_0200(void)
{
    struct __user_cap_data_struct cap_data_data;
    cap_user_data_t cap_data = &cap_data_data;

    int result = capget(NULL, cap_data);
    if (result != -1) {
        t_error("%s capget should be failed\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    capget_0100();
    capget_0200();
    return t_status;
}