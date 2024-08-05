/**
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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
#include <stdlib.h>
#include <dlfcn.h>

#include "rpath_test.h"
#include "functionalext.h"

/**
 * RapthAbsolute - Test dynamic library loading with absolute rpath
 *
 * Tests loading a library with an absolute rpath specified as '/data/tests/libc-test/src/rpath_lib/rpath_support_A'.
 * Verifies if the library 'g_absoluteLib' can be successfully loaded with this rpath configuration.
 */
static void RapthAbsolute(void)
{
    void *handle = dlopen(g_absoluteLib, RTLD_LAZY);
    EXPECT_TRUE("RapthAbsolute", handle);
    if (handle) {
        dlclose(handle);
    }
}

/**
 * RapthRelative - Test dynamic library loading with relative rpath
 *
 * Tests the ability to load a library with a relative rpath specified as '\$ORIGIN/rpath_supportB/../rpath_support_A'.
 * Checks the successful loading of 'g_originLib' using this relative rpath setting.
 */
static void RapthRelative(void)
{
    void *handle = dlopen(g_originLib, RTLD_LAZY);
    EXPECT_TRUE("RapthRelative", handle);
    if (handle) {
        dlclose(handle);
    }
}

/**
 * RapthMultiple - Test dynamic library loading with multiple rpath entries
 *
 * Assesses the ability to load a library with multiple rpath entries,
 * specified as '\$ORIGIN/rpath_supportA:/data/tests/libc-test/src/rpath_lib/rpath_supportB'.
 * Checks if 'g_multipleLib' can be loaded successfully using these combined rpath settings.
 */
static void RapthMultiple(void)
{
    void *handle = dlopen(g_multipleLib, RTLD_LAZY);
    EXPECT_TRUE("RapthMultiple", handle);
    if (handle) {
        dlclose(handle);
    }
}

/**
 * RpathNsOrigin - Test dynamic library loading with relative paths using $ORIGIN
 *
 * This function tests the dynamic library loading mechanism with relative paths based on $ORIGIN.
 * It checks two scenarios:
 * 1. With 'permitted_path' set to '/data/tests/libc-test/src/lib_path/rpath_support_B',
 *    it expects the library load to fail as the actual library is located at
 *    '$ORIGIN/rpath_support_B/../rpath_support_A'.
 * 2. With 'permitted_path' set to '/data/tests/libc-test/src/lib_path', it expects the library load to succeed
 *    since includes the correct path to the library.
 */
static void RpathNsOrigin(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, g_dlName);
    dlns_create2(&dlns, g_dlpath, 0);
    dlns_set_namespace_separated(g_dlName, true);
    dlns_set_namespace_permitted_paths(g_dlName, g_errPath);
    void *handleErr = dlopen_ns(&dlns, g_originLib, RTLD_LAZY);
    EXPECT_FALSE("RpathNsOrigin_err", handleErr);
    if (handleErr) {
        dlclose(handleErr);
    }
    dlns_set_namespace_permitted_paths(g_dlName, g_path);
    void *handle = dlopen_ns(&dlns, g_originLib, RTLD_LAZY);
    EXPECT_TRUE("RpathNsOrigin_correct", handle);
    if (handle) {
        dlclose(handle);
    }
}

/**
 * RpathNsMultiple - Test dynamic library loading with dependencies on multiple .so files
 *
 * The test paths are specified as '/data/tests/libc-test/src/rpath_lib/rpath_support_A:$ORIGIN/../rpath_support_C'.
 * The function sets 'permitted_path' to '/data/tests/libc-test/src/lib_path' and assesses whether the target library
 * can be successfully loaded, given that it has dependencies on both libraries located at the specified paths.
 * This test is critical to verify the handling of multiple rpath entries and their resolution for dependent libraries.
 */
static void RpathNsMultiple(void)
{
    Dl_namespace dlns;
    dlns_init(&dlns, g_dlName);
    dlns_create2(&dlns, g_dlpath, 0);
    dlns_set_namespace_separated(g_dlName, true);
    dlns_set_namespace_permitted_paths(g_dlName, g_path);
    void *handle = dlopen_ns(&dlns, g_multipleLib, RTLD_LAZY);
    EXPECT_TRUE("RpathNsMultiple", handle);
    if (handle) {
        dlclose(handle);
    }
}

TEST_FUN g_gFunArray[] = {
    RapthAbsolute,
    RapthRelative,
    RapthMultiple,
    RpathNsOrigin,
    RpathNsMultiple
};

int main(void)
{
    int num = sizeof(g_gFunArray) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        g_gFunArray[pos]();
    }

    return t_status;
}
