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
 * @tc.name      : dlns_set_namespace_lib_path_0100
 * @tc.desc      : When name is no namespace created, call dlns_set_namespace_lib_path
 * @tc.level     : Level 2
 */
void dlns_set_namespace_lib_path_0100(void)
{
    EXPECT_EQ("dlns_set_namespace_lib_path_0100",
        dlns_set_namespace_lib_path("dlns_set_namespace_lib_path_0100", path), ENOKEY);
}

/**
 * @tc.name      : dlns_set_namespace_lib_path_0200
 * @tc.desc      : When name is a created namespace, call dlns_set_namespace_lib_path
 * @tc.level     : Level 2
 */
void dlns_set_namespace_lib_path_0200(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "dlns_set_namespace_lib_path_0200");

    EXPECT_EQ("dlns_set_namespace_lib_path_0200", dlns_create(&dlns, NULL), EOK);

    void* handle = dlopen_ns(&dlns, dllName_set_002, RTLD_LAZY);
    EXPECT_FALSE("dlns_set_namespace_lib_path_0200", handle);

    EXPECT_EQ("dlns_set_namespace_lib_path_0200",
        dlns_set_namespace_lib_path("dlns_set_namespace_lib_path_0200", path), EOK);

    handle = dlopen_ns(&dlns, dllName_set_002, RTLD_LAZY);
    EXPECT_TRUE("dlns_set_namespace_lib_path_0200", handle);
    dlclose(handle);
}

/**
 * @tc.name      : dlns_set_namespace_lib_path_0300
 * @tc.desc      : When name=NULL, call dlns_set_namespace_lib_path
 * @tc.level     : Level 2
 */
void dlns_set_namespace_lib_path_0300(void)
{
    EXPECT_EQ("dlns_set_namespace_lib_path_0300", dlns_set_namespace_lib_path(NULL, path), EINVAL);
}

/**
 * @tc.name      : dlns_set_namespace_lib_path_0400
 * @tc.desc      : When lib_path=NULL, call dlns_set_namespace_lib_path
 * @tc.level     : Level 2
 */
void dlns_set_namespace_lib_path_0400(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "dlns_set_namespace_lib_path_0400");

    EXPECT_EQ("dlns_set_namespace_lib_path_0400", dlns_create(&dlns, NULL), EOK);

    EXPECT_EQ("dlns_set_namespace_lib_path_0400",
        dlns_set_namespace_lib_path("dlns_set_namespace_lib_path_0400", NULL), EINVAL);
}

/**
 * @tc.name      : dlns_set_namespace_lib_path_0500
 * @tc.desc      : When the lib_path parameter is multiple paths, call dlns_set_namespace_lib_path
 * @tc.level     : Level 1
 */
void dlns_set_namespace_lib_path_0500(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "dlns_set_namespace_lib_path_0500");

    EXPECT_EQ("dlns_set_namespace_lib_path_0500", dlns_create(&dlns, NULL), EOK);

    char* newlibpath = "/data/tests/libc-test/src:/data/tests/libc-test/src/B";

    EXPECT_EQ("dlns_set_namespace_lib_path_0500",
        dlns_set_namespace_lib_path("dlns_set_namespace_lib_path_0500", newlibpath), EOK);

    void *handle = dlopen_ns(&dlns, dllName, RTLD_LAZY);
    EXPECT_TRUE("dlns_set_namespace_lib_path_0500", handle);
    dlclose(handle);

    void *handle1 = dlopen_ns(&dlns, libB, RTLD_LAZY);
    EXPECT_TRUE("dlns_set_namespace_lib_path_0500", handle1);
    dlclose(handle1);
}

/**
 * @tc.name      : dlns_set_namespace_separated_0100
 * @tc.desc      : When name is no namespace created, call dlns_set_namespace_separated
 * @tc.level     : Level 2
 */
void dlns_set_namespace_separated_0100(void)
{
    EXPECT_EQ("dlns_set_namespace_separated_0100",
        dlns_set_namespace_separated("dlns_set_namespace_permitted_paths_0100", true), ENOKEY);
}

/**
 * @tc.name      : dlns_set_namespace_separated_0200
 * @tc.desc      : When the name is a created namespace, call dlns_set_namespace_separated
 * @tc.level     : Level 1
 */
void dlns_set_namespace_separated_0200(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "dlns_set_namespace_separated_0200");

    EXPECT_EQ("dlns_set_namespace_separated_0200", dlns_create(&dlns, NULL), EOK);

    void* handle = dlopen_ns(&dlns, dllNamePath, RTLD_LAZY);
    EXPECT_TRUE("dlns_set_namespace_separated_0200", handle);
    dlclose(handle);

    EXPECT_EQ("dlns_set_namespace_separated_0200",
        dlns_set_namespace_separated("dlns_set_namespace_separated_0200", true), EOK);

    void* handle1 = dlopen_ns(&dlns, dllNamePath, RTLD_LAZY);
    EXPECT_FALSE("dlns_set_namespace_separated_0200", handle1);
}

/**
 * @tc.name      : dlns_set_namespace_separated_0300
 * @tc.desc      : When name=NULL, call dlns_set_namespace_separated
 * @tc.level     : Level 2
 */
void dlns_set_namespace_separated_0300(void)
{
    EXPECT_EQ("dlns_set_namespace_separated_0300", dlns_set_namespace_separated(NULL, true), EINVAL);
}

/**
 * @tc.name      : dlns_set_namespace_permitted_paths_0100
 * @tc.desc      : When name is no namespace created, call dlns_set_namespace_permitted_paths
 * @tc.level     : Level 2
 */
void dlns_set_namespace_permitted_paths_0100(void)
{
    EXPECT_EQ("dlns_set_namespace_permitted_paths_0100",
        dlns_set_namespace_permitted_paths("dlns_set_namespace_permitted_paths_0100", dllNamePath), ENOKEY);
}

/**
 * @tc.name      : dlns_set_namespace_permitted_paths_0200
 * @tc.desc      : name为已创建命名空间时，调用dlns_set_namespace_permitted_paths
 * @tc.level     : Level 2
 */
void dlns_set_namespace_permitted_paths_0200(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "dlns_set_namespace_permitted_paths_0200");

    EXPECT_EQ("dlns_set_namespace_permitted_paths_0200", dlns_create(&dlns, NULL), EOK);

    EXPECT_EQ("dlns_set_namespace_permitted_paths_0200",
        dlns_set_namespace_separated("dlns_set_namespace_permitted_paths_0200", true), EOK);

    void* handle = dlopen_ns(&dlns, dllNamePath, RTLD_LAZY);
    EXPECT_FALSE("dlns_set_namespace_permitted_paths_0200", handle);

    EXPECT_EQ("dlns_set_namespace_permitted_paths_0200",
        dlns_set_namespace_permitted_paths("dlns_set_namespace_permitted_paths_0200", path), EOK);

    void* handle1 = dlopen_ns(&dlns, dllNamePath, RTLD_LAZY);
    EXPECT_TRUE("dlns_set_namespace_permitted_paths_0200", handle1);
    dlclose(handle1);
}

/**
 * @tc.name      : dlns_set_namespace_permitted_paths_0300
 * @tc.desc      : When the name is a created namespace, call dlns_set_namespace_permitted_paths
 * @tc.level     : Level 1
 */
void dlns_set_namespace_permitted_paths_0300(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "dlns_set_namespace_permitted_paths_0300");

    char *newLibPath = "/data/tests/libc-test/src/B/libB.so";

    EXPECT_EQ("dlns_set_namespace_permitted_paths_0300", dlns_create(&dlns, NULL), EOK);

    EXPECT_EQ("dlns_set_namespace_permitted_paths_0300",
        dlns_set_namespace_separated("dlns_set_namespace_permitted_paths_0300", true), EOK);

    void* handle = dlopen_ns(&dlns, dllNamePath, RTLD_LAZY);
    EXPECT_FALSE("dlns_set_namespace_permitted_paths_0300", handle);

    EXPECT_EQ("dlns_set_namespace_permitted_paths_0300",
        dlns_set_namespace_permitted_paths("dlns_set_namespace_permitted_paths_0300", path), EOK);

    void* handle1 = dlopen_ns(&dlns, newLibPath, RTLD_LAZY);
    EXPECT_TRUE("dlns_set_namespace_permitted_paths_0300", handle1);
    dlclose(handle1);
}

/**
 * @tc.name      : dlns_set_namespace_permitted_paths_0400
 * @tc.desc      : When name=NULL, call dlns_set_namespace_permitted_paths
 * @tc.level     : Level 2
 */
void dlns_set_namespace_permitted_paths_0400(void)
{
    EXPECT_EQ("dlns_set_namespace_permitted_paths_0400",
        dlns_set_namespace_permitted_paths(NULL, dllNamePath), EINVAL);
}

/**
 * @tc.name      : dlns_set_namespace_permitted_paths_0500
 * @tc.desc      : When the permitted_paths parameter is multiple paths, call dlns_set_namespace_permitted_paths
 * @tc.level     : Level 1
 */
void dlns_set_namespace_permitted_paths_0500(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "dlns_set_namespace_permitted_paths_0500");

    char* newPermittPath = "/data/tests/libc-test/src";
    char* libBpath = "/data/tests/libc-test/src/B/libB.so";

    EXPECT_EQ("dlns_set_namespace_permitted_paths_0500", dlns_create(&dlns, NULL), EOK);

    EXPECT_EQ("dlns_set_namespace_permitted_paths_0500",
        dlns_set_namespace_separated("dlns_set_namespace_permitted_paths_0500", true), EOK);

    void* handle = dlopen_ns(&dlns, dllNamePath, RTLD_LAZY);
    EXPECT_FALSE("dlns_set_namespace_permitted_paths_0500", handle);

    void* handle1 = dlopen_ns(&dlns, libBpath, RTLD_LAZY);
    EXPECT_FALSE("dlns_set_namespace_permitted_paths_0500", handle1);

    EXPECT_EQ("dlns_set_namespace_permitted_paths_0500",
        dlns_set_namespace_permitted_paths("dlns_set_namespace_permitted_paths_0500", newPermittPath), EOK);

    void* handle2 = dlopen_ns(&dlns, dllNamePath, RTLD_LAZY);
    EXPECT_TRUE("dlns_set_namespace_permitted_paths_0500", handle2);
    dlclose(handle2);

    void* handle3 = dlopen_ns(&dlns, libBpath, RTLD_LAZY);
    EXPECT_TRUE("dlns_set_namespace_permitted_paths_0500", handle3);
    dlclose(handle3);
}

/**
 * @tc.name      : dlns_set_namespace_allowed_libs_0100
 * @tc.desc      : When name is no namespace created, call dlns_set_namespace_allowed_libs
 * @tc.level     : Level 2
 */
void dlns_set_namespace_allowed_libs_0100(void)
{
    // 未创建的命名空间
    EXPECT_EQ("dlns_set_namespace_allowed_libs_0100",
        dlns_set_namespace_allowed_libs("dlns_set_namespace_allowed_libs_0100", path), ENOKEY);
}

/**
 * @tc.name      : dlns_set_namespace_allowed_libs_0200
 * @tc.desc      : When the name is a created namespace, call dlns_set_namespace_allowed_libs
 * @tc.level     : Level 1
 */
void dlns_set_namespace_allowed_libs_0200(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "ns_no_allowed_libs");

    void* handle = dlopen_ns(&dlns, dllNamePath, RTLD_LAZY);
    EXPECT_TRUE("dlns_set_namespace_allowed_libs_0200", handle);
    dlclose(handle);

    EXPECT_EQ("dlns_set_namespace_allowed_libs_0200",
        dlns_set_namespace_allowed_libs("ns_no_allowed_libs", dllName2), EOK);

    void* handle1 = dlopen_ns(&dlns, dllNamePath, RTLD_LAZY);
    EXPECT_FALSE("dlns_set_namespace_allowed_libs_0200", handle1);

    EXPECT_EQ("dlns_set_namespace_allowed_libs_0200",
        dlns_set_namespace_allowed_libs("ns_no_allowed_libs", dllName), EOK);
}

/**
 * @tc.name      : dlns_set_namespace_allowed_libs_0300
 * @tc.desc      : When name=NULL, call dlns_set_namespace_allowed_libs
 * @tc.level     : Level 2
 */
void dlns_set_namespace_allowed_libs_0300(void)
{
    EXPECT_EQ("dlns_set_namespace_allowed_libs_0300", dlns_set_namespace_allowed_libs(NULL, path), EINVAL);
}

/**
 * @tc.name      : dlns_set_namespace_allowed_libs_0400
 * @tc.desc      : When the allowed_libs parameter is multiple file names, call dlns_set_namespace_allowed_libs
 * @tc.level     : Level 1
 */
void dlns_set_namespace_allowed_libs_0400(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "ns_no_allowed_libs");

    void* handle = dlopen_ns(&dlns, dllNamePath, RTLD_LAZY);
    EXPECT_TRUE("dlns_set_namespace_allowed_libs_0400", handle);
    dlclose(handle);

    char* dll4 = "libdlopen_ns_dso.so:libdlopen_dso.so:sharedlibtest.so";

    EXPECT_EQ("dlns_set_namespace_allowed_libs_0400",
        dlns_set_namespace_allowed_libs("ns_no_allowed_libs", dll4), EOK);

    void* handle1 = dlopen_ns(&dlns, dllNamePath, RTLD_LAZY);
    EXPECT_TRUE("dlns_set_namespace_allowed_libs_0400", handle1);
    dlclose(handle1);
}

TEST_FUN G_Fun_Array[] = {
    dlns_set_namespace_lib_path_0100,
    dlns_set_namespace_lib_path_0200,
    dlns_set_namespace_lib_path_0300,
    dlns_set_namespace_lib_path_0400,
    dlns_set_namespace_lib_path_0500,
    dlns_set_namespace_separated_0100,
    dlns_set_namespace_separated_0200,
    dlns_set_namespace_separated_0300,
    dlns_set_namespace_permitted_paths_0100,
    dlns_set_namespace_permitted_paths_0200,
    dlns_set_namespace_permitted_paths_0300,
    dlns_set_namespace_permitted_paths_0400,
    dlns_set_namespace_permitted_paths_0500,
    dlns_set_namespace_allowed_libs_0100,
    dlns_set_namespace_allowed_libs_0200,
    dlns_set_namespace_allowed_libs_0300,
    dlns_set_namespace_allowed_libs_0400,
};

int main(void)
{
    int num = sizeof(G_Fun_Array)/sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}