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

#include "dso_easy_symver.h"

__asm__(".symver dso_easy_symver_old, dso_easy_symver@OLD");
__asm__(".symver dso_easy_symver_stable, dso_easy_symver@@STABLE");
__asm__(".symver dso_easy_symver_dev, dso_easy_symver@DEV");

char *dso_easy_symver_old(void)
{
    return (char *)__FUNCTION__;
}

char *dso_easy_symver_stable(void)
{
    return (char *)__FUNCTION__;
}

char *dso_easy_symver_dev(void)
{
    return (char *)__FUNCTION__;
}
