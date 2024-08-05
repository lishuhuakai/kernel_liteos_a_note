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

#include <dlfcn.h>

#include "dso_symver.h"

/**
 * @tc.name      : reloc_symver_0100
 * @tc.desc      : find versioning symbol while relocating. 
 *                 load libreloc_symver_dso.so that deps on libdso_no_symver.so and libdso_easy_symver.so.
 * @tc.level     : Level 1
 */
static void reloc_symver_0100(void)
{
    symver_log("start");

    void *so = dlopen("libreloc_symver_dso.so", RTLD_NOW);
    if (so) {
        dlclose(so);
    } else {
        symver_error("%s", dlerror());
    }

    symver_log("end");
}

int main(int argc, char *argv[])
{
    symver_log("start");

    reloc_symver_0100();

    symver_log("t_status = %d", t_status);
    symver_log("end");

    return t_status;
}