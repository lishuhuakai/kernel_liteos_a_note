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

static int application_target_sdk_version = SDK_VERSION_FUTURE;

int get_application_target_sdk_version(void)
{
    return application_target_sdk_version;
}

void set_application_target_sdk_version(int target)
{
    if (target == 0) {
        target = SDK_VERSION_FUTURE;
    }
    application_target_sdk_version = target;
}
