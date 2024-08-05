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
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "dlns_test.h"
#include "functionalext.h"


/**
 * @tc.name      : separated_0100
 * @tc.desc      : When the separated property is false, under non-strict isolation, so can be opened by short name
 * @tc.level     : Level 1
 */
void separated_0100(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "ns_separated_flase");
    
    void* handle = dlopen_ns(&dlns, dllName, RTLD_LAZY);
    EXPECT_TRUE("separated_0100", handle);
    dlclose(handle);
}

/**
 * @tc.name      : separated_0200
 * @tc.desc      : When the separated property is false, under non-strict isolation, the wrong path cannot open so
 * @tc.level     : Level 2
 */
void separated_0200(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "ns_separated1");

    // 错误短名称
    void* handle = dlopen_ns(&dlns, dllName2, RTLD_LAZY);
    EXPECT_FALSE("separated_0200", handle);
}

/**
 * @tc.name      : separated_0300
 * @tc.desc      : When the separated property is true, under strict isolation, the name of the incoming
 *     library is under the allowed.libs configuration item, and lib_paths and permitted.paths are configured correctly
 * @tc.level     : Level 1
 */
void separated_0300(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "ns_normal");

    // 长路径
    void* handle = dlopen_ns(&dlns, dllNamePath, RTLD_LAZY);
    EXPECT_TRUE("separated_0300", handle);
    dlclose(handle);
}

/**
 * @tc.name      : separated_0400
 * @tc.desc      : When the separated property is true, under strict isolation, the name of the incoming
 *                  library is under the allowed.libs configuration item, and the lib_paths and permitted.paths
 *                    configurations do not match.
 * @tc.level     : Level 2
 */
void separated_0400(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "ns_wrong_lib_path");

    void* handle = dlopen_ns(&dlns, errdllNamePath, RTLD_LAZY);
    EXPECT_FALSE("separated_0400", handle);
}

/**
 * @tc.name      : separated_0500
 * @tc.desc      : When the separated property is true, under strict isolation, the lib_paths, permitted.paths
 *                  configuration matches,The name of the incoming library is not in the allowed.libs configuration item
 * @tc.level     : Level 2
 */
void separated_0500(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "ns_normal");

    void* handle = dlopen_ns(&dlns, dllNamePath2, RTLD_LAZY);
    EXPECT_FALSE("separated_0500", handle);
}

/**
 * @tc.name      : separated_0600
 * @tc.desc      : When the separated attribute is false, under non-strict isolation,
 *                  calling dlopen_ns can open so by short name
 * @tc.level     : Level 2
 */
void separated_0600(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "separated_0600");

    EXPECT_EQ("separated_0600", dlns_create(&dlns, path), EOK);
    EXPECT_EQ("separated_0600", dlns_set_namespace_separated("separated_0600", false), EOK);

    void* handle = dlopen_ns(&dlns, dllName, RTLD_LAZY);
    EXPECT_TRUE("separated_0600", handle);
    dlclose(handle);
}

/**
 * @tc.name      : separated_0700
 * @tc.desc      : When the separated property is false, under non-strict isolation,
 *                  calling dlopen_ns error short name cannot open so
 */
void separated_0700(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "separated_0700");

    EXPECT_EQ("separated_0700", dlns_create(&dlns, path), EOK);
    EXPECT_EQ("separated_0700", dlns_set_namespace_separated("separated_0700", false), EOK);

    void* handle = dlopen_ns(&dlns, errPath_ns, RTLD_LAZY);
    EXPECT_FALSE("separated_0700", handle);
}

/**
 * @tc.name      : separated_0800
 * @tc.desc      : When the separated property is true, under strict isolation, the name of the incoming library is in
 *                  the allowed.libs configuration item, and the lib_paths and permitted.paths configurations match.
 *                    Calling dlopen_ns can open so through the full path.
 * @tc.level     : Level 1
 */
void separated_0800(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "separated_0800");

    EXPECT_EQ("separated_0800", dlns_create(&dlns, path), EOK);
    EXPECT_EQ("separated_0800", dlns_set_namespace_separated("separated_0800", true), EOK);
    EXPECT_EQ("separated_0800", dlns_set_namespace_permitted_paths("separated_0800", dllNamePath), EOK);
    EXPECT_EQ("separated_0800", dlns_set_namespace_lib_path("separated_0800", path), EOK);
    EXPECT_EQ("separated_0800", dlns_set_namespace_allowed_libs("separated_0800", dllName), EOK);

    void* handle = dlopen_ns(&dlns, dllNamePath, RTLD_LAZY);
    EXPECT_TRUE("separated_0800", handle);
    dlclose(handle);
}

/**
 * @tc.name      : separated_0900
 * @tc.desc      : When the separated property is true, under strict isolation, the name of the incoming library is
 *                  in the allowed.libs configuration item, and the lib_paths, permitted.paths configuration does not
 *                      match, and calling dlopen_ns cannot open so through the full path
 * @tc.level     : Level 2
 */
void separated_0900(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "separated_0900");

    EXPECT_EQ("separated_0900", dlns_create(&dlns, path), EOK);
    EXPECT_EQ("separated_0900", dlns_set_namespace_separated("separated_0900", true), EOK);
    EXPECT_EQ("separated_0900", dlns_set_namespace_permitted_paths("separated_0900", errdllNamePath), EOK);
    EXPECT_EQ("separated_0900", dlns_set_namespace_lib_path("separated_0900", errPath_ns), EOK);
    EXPECT_EQ("separated_0900", dlns_set_namespace_allowed_libs("separated_0900", dllName_sep_009), EOK);

    void* handle = dlopen_ns(&dlns, dllName_sep_009, RTLD_LAZY);
    EXPECT_FALSE("separated_0900", handle);
}

/**
 * @tc.name      : separated_1000
 * @tc.desc      : When the separated property is true, under strict isolation, the lib_paths, permitted.paths
 *                  configuration matches,The name of the incoming library is not in the allowed.libs configuration
 *                      item, calling dlopen_ns cannot open so through the full path
 * @tc.level     : Level 2
 */
void separated_1000(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "separated_1000");

    EXPECT_EQ("separated_1000", dlns_create(&dlns, path), EOK);
    EXPECT_EQ("separated_1000", dlns_set_namespace_separated("separated_1000", true), EOK);
    EXPECT_EQ("separated_1000", dlns_set_namespace_permitted_paths("separated_1000", dllNamePath), EOK);
    EXPECT_EQ("separated_1000", dlns_set_namespace_lib_path("separated_1000", path), EOK);
    EXPECT_EQ("separated_1000", dlns_set_namespace_allowed_libs("separated_1000", dllName2), EOK);

    void* handle = dlopen_ns(&dlns, dllNamePath, RTLD_LAZY);
    EXPECT_FALSE("separated_1000", handle);
}

/**
 * @tc.name      : separated_1100
 * @tc.desc      : The asan.lib.paths path will not be used when the Asan function is not enabled
 * @tc.level     : Level 2
 */
void separated_1100(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "ns_asan_lib_path");

    void* handle = dlopen_ns(&dlns, dllName, RTLD_LAZY);
    EXPECT_TRUE("separated_1100", handle);
    dlclose(handle);

    void* handle1 = dlopen_ns(&dlns, libB, RTLD_LAZY);
    EXPECT_FALSE("separated_1100", handle1);
}

/**
 * @tc.name      : separated_1200
 * @tc.desc      : asan.permitted.paths paths will not be used when the Asan function is not enabled
 * @tc.level     : Level 2
 */
void separated_1200(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "ns_asan_permit_path");

    char *libcc = "/data/tests/libc-test/src/C/libC.so";

    void* handle = dlopen_ns(&dlns, libcc, RTLD_LAZY);
    EXPECT_TRUE("separated_1200", handle);
    dlclose(handle);

    char *libbb = "/data/tests/libc-test/src/B/libB.so";

    void* handle1 = dlopen_ns(&dlns, libbb, RTLD_LAZY);
    EXPECT_FALSE("separated_1200", handle1);
}

TEST_FUN G_Fun_Array[] = {
    separated_0100,
    separated_0200,
    separated_0300,
    separated_0400,
    separated_0500,
    separated_0600,
    separated_0700,
    separated_0800,
    separated_0900,
    separated_1000,
    separated_1100,
    separated_1200,
};

int main(void)
{
    int num = sizeof(G_Fun_Array)/sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }
    return t_status;
}