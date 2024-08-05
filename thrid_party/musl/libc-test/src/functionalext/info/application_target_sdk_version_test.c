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

#include <info/application_target_sdk_version.h>

#include <string.h>
#include <test.h>

// val1 == val2
#define EXPECT_EQ(a, b) do { \
    if ((a) != (b)) \
        t_error("failed %d != %d \n", a, b); \
} while (0)
typedef void (*TEST_FUN)(void);

/**
 * @tc.name      : set_application_target_sdk_version
 * @tc.desc      : Test the function of set_application_target_sdk_version.
 * @tc.level     : Level 0
 */

static void set_application_target_sdk_version_0010(void)
{
    set_application_target_sdk_version(SDK_VERSION_7);
    int target = get_application_target_sdk_version();
    EXPECT_EQ(target, SDK_VERSION_7);
}

/**
 * @tc.name      : get_application_target_sdk_version
 * @tc.desc      : Test the function of get_application_target_sdk_version.
 * @tc.level     : Level 0
 */
static void get_application_target_sdk_version_0010(void)
{
    set_application_target_sdk_version(SDK_VERSION_8);
    int target = get_application_target_sdk_version();
    EXPECT_EQ(target, SDK_VERSION_8);
}

/**
 * @tc.name      : set_application_target_sdk_version
 * @tc.desc      : Test the function of set_application_target_sdk_version.
 * @tc.level     : Level 0
 */
static void set_application_target_sdk_version_0020(void)
{
    set_application_target_sdk_version(0);
    int target = get_application_target_sdk_version();
    EXPECT_EQ(target, SDK_VERSION_FUTURE);
}

TEST_FUN G_Fun_Array[] = {
    set_application_target_sdk_version_0010,
    get_application_target_sdk_version_0010,
    set_application_target_sdk_version_0020
};

int main(void)
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}