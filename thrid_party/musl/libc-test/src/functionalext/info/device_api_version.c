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

#include <info/device_api_version.h>

#include <string.h>
#include <test.h>

#define EXPECT_EQ(a, b) do { \
    if ((a) != (b)) \
        t_error("failed %d != %d \n", a, b); \
} while (0)

#define API_VERSION_DEFAULT    0

/**
 * @tc.name      : get_device_api_version
 * @tc.desc      : Test the function of get_device_api_version.
 * @tc.level     : Level 0
 */
static void get_device_api_version_0010(void)
{
    int APIVersion = get_device_api_version();
    EXPECT_EQ(APIVersion, API_VERSION_DEFAULT);
}
int main(void)
{
    get_device_api_version_0010();

    return t_status;
}
