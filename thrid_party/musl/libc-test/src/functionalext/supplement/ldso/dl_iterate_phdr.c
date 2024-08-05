/**
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

#include <dlfcn.h>
#include <link.h>
#include <stdlib.h>
#include "functionalext.h"

static int header_handler(struct dl_phdr_info *info, size_t size, void *data)
{
    void *addrhead = (void *)(info->dlpi_addr + info->dlpi_phdr[0].p_vaddr);
    if ((strcmp(info->dlpi_name, "/data/libtest.so") == 0) && (addrhead != NULL)) {
        return 1;
    }
    return 0;
}

/**
 * @tc.name      : dl_iterate_phdr_0100
 * @tc.desc      : The parameter is valid, get the starting address of the dynamic library loaded in the memory.
 * @tc.level     : Level 0
 */
void dl_iterate_phdr_0100(void)
{
    system("cp /system/lib/libutils.z.so /data/libtest.so");
    void *handle = dlopen("/data/libtest.so", RTLD_NOW);
    int ret = 0;
    ret = dl_iterate_phdr(header_handler, NULL);
    EXPECT_EQ("dl_iterate_phdr_0100", (int)ret, 1);
    system("rm -rf /data/libtest.so");
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    dl_iterate_phdr_0100();
    return t_status;
}