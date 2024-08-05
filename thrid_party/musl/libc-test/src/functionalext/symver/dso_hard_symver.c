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

#include "dso_hard_symver.h"

__asm__(".symver dso_hard_symver_if_old, dso_hard_symver_if@OLD");
__asm__(".symver dso_hard_symver_if_stable, dso_hard_symver_if@@STABLE");
__asm__(".symver dso_hard_symver_if_dev, dso_hard_symver_if@DEV");
__asm__(".symver dso_hard_symver_ld_old, dso_hard_symver_ld@OLD");
__asm__(".symver dso_hard_symver_ld_stable, dso_hard_symver_ld@@STABLE");
__asm__(".symver dso_hard_symver_ld_dev, dso_hard_symver_ld@DEV");

char *dso_hard_symver_if_old(void)
{
    return (char *)__FUNCTION__;
}

char *dso_hard_symver_if_stable(void)
{
    return (char *)__FUNCTION__;
}

char *dso_hard_symver_if_dev(void)
{
    return (char *)__FUNCTION__;
}

char *dso_hard_symver_ld_old(void)
{
    return (char *)__FUNCTION__;
}

char *dso_hard_symver_ld_stable(void)
{
    return (char *)__FUNCTION__;
}

char *dso_hard_symver_ld_dev(void)
{
    return (char *)__FUNCTION__;
}
