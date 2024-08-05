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
 * @tc.name      : dlns_inherit_0100
 * @tc.desc      : Under the INI configuration file, when shared_libs in dlns_inherit is empty,
 *                      all the libraries of the inheritor can be shared by it
 * @tc.level     : Level 1
 */
void dlns_inherit_0100(void)
{
    Dl_namespace dlnsA, dlnsB;
    dlns_init(&dlnsA, "for_inherit_A");
    dlns_init(&dlnsB, "inherited_class");

    EXPECT_EQ("dlns_inherit_0100", dlns_inherit(&dlnsA, &dlnsB, NULL), EOK);

    void* handle = dlopen_ns(&dlnsA, dllName, RTLD_LAZY);
    EXPECT_TRUE("dlns_inherit_0100", handle);
    dlclose(handle);

    void* handle1 = dlopen_ns(&dlnsA, sharedLib, RTLD_LAZY);
    EXPECT_TRUE("dlns_inherit_0100", handle1);
    dlclose(handle1);
}

/**
 * @tc.name      : dlns_inherit_0200
 * @tc.desc      : Under the INI configuration file, when shared_libs in dlns_inherit is set, the set shared library
 *                      name can be shared, and other than the shared library name, it will not be shared
 * @tc.level     : Level 2
 */
void dlns_inherit_0200(void)
{
    Dl_namespace dlnsA, dlnsB;
    dlns_init(&dlnsA, "for_inherit_A");
    dlns_init(&dlnsB, "inherited_class");

    EXPECT_EQ("dlns_inherit_0200", dlns_inherit(&dlnsA, &dlnsB, dllName), EOK);

    void* handle = dlopen_ns(&dlnsA, dllName, RTLD_LAZY);
    EXPECT_TRUE("dlns_inherit_0200", handle);
    dlclose(handle);

    void* handle1 = dlopen_ns(&dlnsA, sharedLib, RTLD_LAZY);
    EXPECT_FALSE("dlns_inherit_0200", handle1);
}

/**
 * @tc.name      : dlns_inherit_0300
 * @tc.desc      : Under the INI configuration file, the library that cannot be opened
 *                      by itself is opened through inheritance
 * @tc.level     : Level 1
 */
void dlns_inherit_0300(void)
{
    Dl_namespace dlnsA, dlnsB;
    dlns_init(&dlnsA, "inherit_A");
    dlns_init(&dlnsB, "inherited_class");

    EXPECT_EQ("dlns_inherit_0300", dlns_create(&dlnsA, NULL), EOK);

    void* handle1 = dlopen_ns(&dlnsA, dllName_inh_003, RTLD_LAZY);
    EXPECT_FALSE("dlns_inherit_0300", handle1);

    EXPECT_EQ("dlns_inherit_0300", dlns_inherit(&dlnsA, &dlnsB, dllName_inh_003), EOK);

    void* handle = dlopen_ns(&dlnsA, dllName_inh_003, RTLD_LAZY);
    EXPECT_TRUE("dlns_inherit_0300", handle);
    dlclose(handle);
}

/**
 * @tc.name      : dlns_inherit_0400
 * @tc.desc      : In the INI configuration file, inheritance cannot be passed, and cannot be opened after generation
 * @tc.level     : Level 2
 */
void dlns_inherit_0400(void)
{
    Dl_namespace dlnsA, dlnsAA, dlnsB;
    dlns_init(&dlnsA, "for_inherit_A");
    dlns_init(&dlnsAA, "for_inherit_AA");
    dlns_init(&dlnsB, "inherited_class");

    EXPECT_EQ("dlns_inherit_0400", dlns_inherit(&dlnsA, &dlnsB, dllName), EOK);

    void* handle = dlopen_ns(&dlnsA, dllName, RTLD_LAZY);
    EXPECT_PTRNE("dlns_inherit_0400", handle, NULL);
    dlclose(handle);

    EXPECT_EQ("dlns_inherit_0400", dlns_inherit(&dlnsAA, &dlnsA, NULL), EOK);

    void* handle1 = dlopen_ns(&dlnsAA, dllName, RTLD_LAZY);
    EXPECT_FALSE("dlns_inherit_0400", handle1);
}

/**
 * @tc.name      : dlns_inherit_0500
 * @tc.desc      : Create ns through the interface, when shared_libs in dlns_inherit is empty,
 *                  all the libraries of the inheritor can be shared by it
 * @tc.level     : Level 1
 */
void dlns_inherit_0500(void)
{
    Dl_namespace dlnsA, dlnsB;
    dlns_init(&dlnsA, "dlns_inherit_0500_A");
    dlns_init(&dlnsB, "dlns_inherit_0500_B");

    EXPECT_EQ("dlns_inherit_0500", dlns_create(&dlnsA, NULL), EOK);
    EXPECT_EQ("dlns_inherit_0500", dlns_create(&dlnsB, path), EOK);

    EXPECT_EQ("dlns_inherit_0500", dlns_set_namespace_separated("dlns_inherit_0500_A", true), EOK);
    EXPECT_EQ("dlns_inherit_0500", dlns_set_namespace_separated("dlns_inherit_0500_B", true), EOK);

    EXPECT_EQ("dlns_inherit_0500", dlns_inherit(&dlnsA, &dlnsB, NULL), EOK);

    void* handle = dlopen_ns(&dlnsA, dllName, RTLD_LAZY);
    EXPECT_TRUE("dlns_inherit_0500", handle);
    dlclose(handle);

    handle = dlopen_ns(&dlnsA, sharedLib, RTLD_LAZY);
    EXPECT_TRUE("dlns_inherit_0500", handle);
    dlclose(handle);
}

/**
 * @tc.name      : dlns_inherit_0600
 * @tc.desc      : Create ns through the interface, when shared_libs in dlns_inherit is set, the set shared
 *                  library name can be shared, other than the shared library name, it will not be shared
 * @tc.level     : Level 2
 */
void dlns_inherit_0600(void)
{
    Dl_namespace dlnsA, dlnsB;
    dlns_init(&dlnsA, "dlns_inherit_0600_A");
    dlns_init(&dlnsB, "dlns_inherit_0600_B");

    EXPECT_EQ("dlns_inherit_0600", dlns_create(&dlnsA, NULL), EOK);
    EXPECT_EQ("dlns_inherit_0600", dlns_create(&dlnsB, path), EOK);

    EXPECT_EQ("dlns_inherit_0600", dlns_set_namespace_separated("dlns_inherit_0600_A", true), EOK);
    EXPECT_EQ("dlns_inherit_0600", dlns_set_namespace_separated("dlns_inherit_0600_B", true), EOK);

    EXPECT_EQ("dlns_inherit_0600", dlns_inherit(&dlnsA, &dlnsB, dllName), EOK);

    void* handle = dlopen_ns(&dlnsA, dllName, RTLD_LAZY);
    EXPECT_TRUE("dlns_inherit_0600", handle);
    dlclose(handle);

    handle = dlopen_ns(&dlnsA, sharedLib, RTLD_LAZY);
    EXPECT_FALSE("dlns_inherit_0600", handle);
}

/**
 * @tc.name      : dlns_inherit_0700
 * @tc.desc      : Create ns through interfaces, and open libraries that cannot be
 *                  opened by themselves through inheritance.
 * @tc.level     : Level 2
 */
void dlns_inherit_0700(void)
{
    Dl_namespace dlnsA, dlnsB;
    dlns_init(&dlnsA, "dlns_inherit_0700_A");
    dlns_init(&dlnsB, "dlns_inherit_0700_B");

    EXPECT_EQ("dlns_inherit_0700", dlns_create(&dlnsA, NULL), EOK);
    EXPECT_EQ("dlns_inherit_0700", dlns_create(&dlnsB, path), EOK);

    EXPECT_EQ("dlns_inherit_0700", dlns_set_namespace_separated("dlns_inherit_0700_A", true), EOK);
    EXPECT_EQ("dlns_inherit_0700", dlns_set_namespace_separated("dlns_inherit_0700_B", true), EOK);

    EXPECT_EQ("dlns_inherit_0700", dlns_set_namespace_allowed_libs("dlns_inherit_0700_A", dllName2), EOK);

    void* handle = dlopen_ns(&dlnsA, dllName_inh_007, RTLD_LAZY);
    EXPECT_FALSE("dlns_inherit_0700", handle);

    EXPECT_EQ("dlns_inherit_0700", dlns_inherit(&dlnsA, &dlnsB, NULL), EOK);

    handle = dlopen_ns(&dlnsA, dllName_inh_007, RTLD_LAZY);
    EXPECT_TRUE("dlns_inherit_0700", handle);
    dlclose(handle);
}

/**
 * @tc.name      : dlns_inherit_0800
 * @tc.desc      : Create ns through an interface, inheritance cannot be passed,
 *                      and cannot be opened between generations.
 * @tc.level     : Level 2
 */
void dlns_inherit_0800(void)
{
    Dl_namespace dlnsA, dlnsB, dlnsC;
    dlns_init(&dlnsA, "dlns_inherit_0800_A");
    dlns_init(&dlnsB, "dlns_inherit_0800_B");
    dlns_init(&dlnsC, "dlns_inherit_0800_C");

    EXPECT_EQ("dlns_inherit_0800", dlns_create(&dlnsA, NULL), EOK);
    EXPECT_EQ("dlns_inherit_0800", dlns_create(&dlnsB, NULL), EOK);
    EXPECT_EQ("dlns_inherit_0800", dlns_create(&dlnsC, path), EOK);

    EXPECT_EQ("dlns_inherit_0800", dlns_set_namespace_separated("dlns_inherit_0800_A", true), EOK);
    EXPECT_EQ("dlns_inherit_0800", dlns_set_namespace_separated("dlns_inherit_0800_B", true), EOK);
    EXPECT_EQ("dlns_inherit_0800", dlns_set_namespace_separated("dlns_inherit_0800_C", true), EOK);

    // A继承B, B继承C
    EXPECT_EQ("dlns_inherit_0800", dlns_inherit(&dlnsB, &dlnsC, NULL), EOK);
    EXPECT_EQ("dlns_inherit_0800", dlns_inherit(&dlnsA, &dlnsB, NULL), EOK);

    void* handle = dlopen_ns(&dlnsB, dllName_inh_008, RTLD_LAZY);
    EXPECT_TRUE("dlns_inherit_0800", handle);
    dlclose(handle);

    handle = dlopen_ns(&dlnsA, dllName_inh_008, RTLD_LAZY);
    EXPECT_FALSE("dlns_inherit_0800", handle);
}

/**
 * @tc.name      : dlns_inherit_0900
 * @tc.desc      : Inherit multiple namespaces
 * @tc.level     : Level 2
 */
void dlns_inherit_0900(void)
{
    Dl_namespace dlnsA, dlnsB, dlnsC, dlnsD;
    dlns_init(&dlnsA, "dlns_inherit_0900_A");
    dlns_init(&dlnsB, "dlns_inherit_0900_B");
    dlns_init(&dlnsC, "dlns_inherit_0900_C");
    dlns_init(&dlnsD, "dlns_inherit_0900_D");

    EXPECT_EQ("dlns_inherit_0900", dlns_create(&dlnsA, NULL), EOK);
    EXPECT_EQ("dlns_inherit_0900", dlns_create(&dlnsB, pathB), EOK);
    EXPECT_EQ("dlns_inherit_0900", dlns_create(&dlnsC, pathC), EOK);
    EXPECT_EQ("dlns_inherit_0900", dlns_create(&dlnsD, pathD), EOK);

    EXPECT_EQ("dlns_inherit_0900", dlns_set_namespace_separated("dlns_inherit_0900_A", true), EOK);
    EXPECT_EQ("dlns_inherit_0900", dlns_set_namespace_separated("dlns_inherit_0900_B", true), EOK);
    EXPECT_EQ("dlns_inherit_0900", dlns_set_namespace_separated("dlns_inherit_0900_C", true), EOK);
    EXPECT_EQ("dlns_inherit_0900", dlns_set_namespace_separated("dlns_inherit_0900_D", true), EOK);

    EXPECT_EQ("dlns_inherit_0900", dlns_inherit(&dlnsA, &dlnsB, NULL), EOK);
    EXPECT_EQ("dlns_inherit_0900", dlns_inherit(&dlnsA, &dlnsC, NULL), EOK);
    EXPECT_EQ("dlns_inherit_0900", dlns_inherit(&dlnsA, &dlnsD, NULL), EOK);

    void* handle1 = dlopen_ns(&dlnsA, libB, RTLD_LAZY);
    EXPECT_TRUE("dlns_inherit_0900", handle1);
    dlclose(handle1);

    void* handle2 = dlopen_ns(&dlnsA, libC, RTLD_LAZY);
    EXPECT_TRUE("dlns_inherit_0900", handle2);
    dlclose(handle2);

    void* handle3 = dlopen_ns(&dlnsA, libD, RTLD_LAZY);
    EXPECT_TRUE("dlns_inherit_0900", handle3);
    dlclose(handle3);
}

/**
 * @tc.name      : dlns_inherit_1000
 * @tc.desc      : Inherited by multiple namespaces
 * @tc.level     : Level 2
 */
void dlns_inherit_1000(void)
{
    Dl_namespace dlnsA, dlnsB, dlnsC, dlnsD;
    dlns_init(&dlnsA, "dlns_inherit_1000_A");
    dlns_init(&dlnsB, "dlns_inherit_1000_B");
    dlns_init(&dlnsC, "dlns_inherit_1000_C");
    dlns_init(&dlnsD, "dlns_inherit_1000_D");

    EXPECT_EQ("dlns_inherit_1000", dlns_create(&dlnsA, path), EOK);
    EXPECT_EQ("dlns_inherit_1000", dlns_create(&dlnsB, NULL), EOK);
    EXPECT_EQ("dlns_inherit_1000", dlns_create(&dlnsC, NULL), EOK);
    EXPECT_EQ("dlns_inherit_1000", dlns_create(&dlnsD, NULL), EOK);

    EXPECT_EQ("dlns_inherit_1000", dlns_set_namespace_separated("dlns_inherit_1000_A", true), EOK);
    EXPECT_EQ("dlns_inherit_1000", dlns_set_namespace_separated("dlns_inherit_1000_B", true), EOK);
    EXPECT_EQ("dlns_inherit_1000", dlns_set_namespace_separated("dlns_inherit_1000_C", true), EOK);
    EXPECT_EQ("dlns_inherit_1000", dlns_set_namespace_separated("dlns_inherit_1000_D", true), EOK);

    EXPECT_EQ("dlns_inherit_1000", dlns_inherit(&dlnsB, &dlnsA, NULL), EOK);
    EXPECT_EQ("dlns_inherit_1000", dlns_inherit(&dlnsC, &dlnsA, NULL), EOK);
    EXPECT_EQ("dlns_inherit_1000", dlns_inherit(&dlnsD, &dlnsA, NULL), EOK);

    void* handle1 = dlopen_ns(&dlnsB, dllName, RTLD_LAZY);
    EXPECT_TRUE("dlns_inherit_1000", handle1);
    dlclose(handle1);

    void* handle2 = dlopen_ns(&dlnsC, dllName, RTLD_LAZY);
    EXPECT_TRUE("dlns_inherit_1000", handle2);
    dlclose(handle2);

    void* handle3 = dlopen_ns(&dlnsD, dllName, RTLD_LAZY);
    EXPECT_TRUE("dlns_inherit_1000", handle3);
    dlclose(handle3);
}

/**
 * @tc.name      : dlns_inherit_1100
 * @tc.desc      : When repeating inheritance, replace the original inheritance relationship
 * @tc.level     : Level 2
 */
void dlns_inherit_1100(void)
{
    Dl_namespace dlnsA, dlnsB;
    dlns_init(&dlnsA, "dlns_inherit_1100_A");
    dlns_init(&dlnsB, "dlns_inherit_1100_B");

    EXPECT_EQ("dlns_inherit_1100", dlns_create(&dlnsA, NULL), EOK);
    EXPECT_EQ("dlns_inherit_1100", dlns_create(&dlnsB, NULL), EOK);

    EXPECT_EQ("dlns_inherit_1100", dlns_set_namespace_separated("dlns_inherit_1100_A", true), EOK);
    EXPECT_EQ("dlns_inherit_1100", dlns_set_namespace_separated("dlns_inherit_1100_B", true), EOK);

    EXPECT_EQ("dlns_inherit_1100", dlns_inherit(&dlnsA, &dlnsB, NULL), EOK);
    void* handle = dlopen_ns(&dlnsA, dllName_inh_011, RTLD_LAZY);
    EXPECT_FALSE("dlns_inherit_1100", handle);

    EXPECT_EQ("dlns_inherit_1100", dlns_set_namespace_allowed_libs("dlns_inherit_1100_B", dllName_inh_011), EOK);
    EXPECT_EQ("dlns_inherit_1100", dlns_set_namespace_permitted_paths("dlns_inherit_1100_B", dllNamePath), EOK);
    EXPECT_EQ("dlns_inherit_1100", dlns_set_namespace_lib_path("dlns_inherit_1100_B", path), EOK);

    // AGIN
    EXPECT_EQ("dlns_inherit_1100", dlns_inherit(&dlnsA, &dlnsB, NULL), EOK);

    handle = dlopen_ns(&dlnsA, dllName_inh_011, RTLD_LAZY);
    EXPECT_TRUE("dlns_inherit_1100", handle);
    dlclose(handle);
}

TEST_FUN G_Fun_Array[] = {
    dlns_inherit_0100,
    dlns_inherit_0200,
    dlns_inherit_0300,
    dlns_inherit_0400,
    dlns_inherit_0500,
    dlns_inherit_0600,
    dlns_inherit_0700,
    dlns_inherit_0800,
    dlns_inherit_0900,
    dlns_inherit_1000,
    dlns_inherit_1100,
};
int main(void)
{
    int num = sizeof(G_Fun_Array)/sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}